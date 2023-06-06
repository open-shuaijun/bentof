/*****************************************************************
|
|    AP4 - MP4 to HLS File Converter
|
|    Copyright 2002-2018 Axiomatic Systems, LLC
|
|
|    This file is part of Bento4/AP4 (MP4 Atom Processing Library).
|
|    Unless you have obtained Bento4 under a difference license,
|    this version of Bento4 is Bento4|GPL.
|    Bento4|GPL is free software; you can redistribute it and/or modify
|    it under the terms of the GNU General Public License as published by
|    the Free Software Foundation; either version 2, or (at your option)
|    any later version.
|
|    Bento4|GPL is distributed in the hope that it will be useful,
|    but WITHOUT ANY WARRANTY; without even the implied warranty of
|    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|    GNU General Public License for more details.
|
|    You should have received a copy of the GNU General Public License
|    along with Bento4|GPL; see the file COPYING.  If not, write to the
|    Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
|    02111-1307, USA.
|
 ****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/

#include "Mp42Hls.h"


/*----------------------------------------------------------------------
|   PrintUsageAndExit
+---------------------------------------------------------------------*/
static void
PrintUsageAndExit() {
    fprintf(stderr,
            BANNER
            "\n\nusage: mp42hls [options] <input>\n"
            "HlsOptions:\n"
            "  --verbose\n"
            "  --show-info\n"
            "  --hls-version <n> (default: 3)\n"
            "  --pmt-pid <pid>\n"
            "    PID to use for the PMT (default: 0x100)\n"
            "  --audio-pid <pid>\n"
            "    PID to use for audio packets (default: 0x101)\n"
            "  --video-pid <pid>\n"
            "    PID to use for video packets (default: 0x102)\n"
            "  --audio-track-id <n>\n"
            "    Read audio from track ID <n> (0 means no audio)\n"
            "  --video-track-id <n>\n"
            "    Read video from track ID <n> (0 means no video)\n"
            "  --audio-format <format>\n"
            "    Format to use for audio-only segments: 'ts' or 'packed' (default: 'ts')\n"
            "  --segment-duration <n>\n"
            "    Target segment duration in seconds (default: 6)\n"
            "  --segment-duration-threshold <t>\n"
            "    Segment duration threshold in milliseconds (default: 15)\n"
            "  --pcr-offset <offset> in units of 90kHz (default 10000)\n"
            "  --index-filename <filename>\n"
            "    Filename to use for the playlist/index (default: stream.m3u8)\n"
            "  --allow-cache <YES|NO>\n"
            "    set #EXT-X-ALLOW-CACHE to YES or NO\n"
            "  --segment-filename-template <pattern>\n"
            "    Filename pattern to use for the segments. Use a printf-style pattern with\n"
            "    one number field for the segment number, unless using single file mode\n"
            "    (default: segment-%%d.<ext> for separate segment files, or stream.<ext> for single file)\n"
            "  --segment-url-template <pattern>\n"
            "    URL pattern to use for the segments. Use a printf-style pattern with\n"
            "    one number field for the segment number unless unsing single file mode.\n"
            "    (may be a relative or absolute URI).\n"
            "    (default: segment-%%d.<ext> for separate segment files, or stream.<ext> for single file)\n"
            "  --iframe-index-filename <filename>\n"
            "    Filename to use for the I-Frame playlist (default: iframes.m3u8 when HLS version >= 4)\n"
            "  --output-single-file\n"
            "    Output all the media in a single file instead of separate segment files.\n"
            "    The segment filename template and segment URL template must be simple strings\n"
            "    without '%%d' or other printf-style patterns\n"
            "  --encryption-mode <mode>\n"
            "    Encryption mode (only used when --encryption-key is specified). AES-128 or SAMPLE-AES (default: AES-128)\n"
            "  --encryption-key <key>\n"
            "    Encryption key in hexadecimal (default: no encryption)\n"
            "  --encryption-iv-mode <mode>\n"
            "    Encryption IV mode: 'sequence', 'random' or 'fps' (FairPlay Streaming) (default: sequence)\n"
            "    (when the mode is 'fps', the encryption key must be 32 bytes: 16 bytes for the key\n"
            "    followed by 16 bytes for the IV).\n"
            "  --encryption-key-uri <uri>\n"
            "    Encryption key URI (may be a realtive or absolute URI). (default: key.bin)\n"
            "  --encryption-key-format <format>\n"
            "    Encryption key format. (default: 'identity')\n"
            "  --encryption-key-format-versions <versions>\n"
            "    Encryption key format versions.\n"
            "  --encryption-key-line <ext-x-key-line>\n"
            "    Preformatted encryption key line (only the portion after the #EXT-X-KEY: tag).\n"
            "    This option can be used multiple times, once for each preformatted key line to be included in the playlist.\n"
            "    (this option is mutually exclusive with the --encryption-key-uri, --encryption-key-format and --encryption-key-format-versions options)\n"
            "    (the IV and METHOD parameters will automatically be added, so they must not appear in the <ext-x-key-line> argument)\n"
    );
    exit(1);
}


/*----------------------------------------------------------------------
|   TrackSampleReader
+---------------------------------------------------------------------*/
AP4_Result
TrackSampleHlsReader::ReadSample(AP4_Sample &sample, AP4_DataBuffer &sample_data) {
    if (m_SampleIndex >= m_Track.GetSampleCount()) return AP4_ERROR_EOS;
    return m_Track.ReadSample(m_SampleIndex++, sample, sample_data);
}

/*----------------------------------------------------------------------
|   FragmentedSampleReader
+---------------------------------------------------------------------*/
AP4_Result
FragmentedSampleHlsReader::ReadSample(AP4_Sample &sample, AP4_DataBuffer &sample_data) {
    return m_FragmentReader.ReadNextSample(m_TrackId, sample, sample_data);
}

/*----------------------------------------------------------------------
|   OpenOutput
+---------------------------------------------------------------------*/
static AP4_ByteStream *
OpenOutput(const char *filename_pattern, unsigned int segment_number) {
    AP4_ByteStream *output = NULL;
    char filename[4096];
    sprintf(filename, filename_pattern, segment_number);
    AP4_Result result = AP4_FileByteStream::Create(filename, AP4_FileByteStream::STREAM_MODE_WRITE,
                                                   output);
    if (AP4_FAILED(result)) {
        char msg[256];
        sprintf(msg, "ERROR: cannot open output (%d)\n", result);
        __android_log_print(ANDROID_LOG_DEBUG, "TAG", "失败的文件:%s  filename=%s", msg, filename);
        return NULL;
    }

    return output;
}

/*----------------------------------------------------------------------
|   PreventStartCodeEmulation
+---------------------------------------------------------------------*/
static void
PreventStartCodeEmulation(const AP4_UI08 *payload, AP4_Size payload_size, AP4_DataBuffer &output) {
    output.Reserve(payload_size * 2); // more than enough
    AP4_Size output_size = 0;
    AP4_UI08 *buffer = output.UseData();

    unsigned int zero_counter = 0;
    for (unsigned int i = 0; i < payload_size; i++) {
        if (zero_counter == 2) {
            if (payload[i] == 0 || payload[i] == 1 || payload[i] == 2 || payload[i] == 3) {
                buffer[output_size++] = 3;
                zero_counter = 0;
            }
        }

        buffer[output_size++] = payload[i];

        if (payload[i] == 0) {
            ++zero_counter;
        } else {
            zero_counter = 0;
        }
    }

    output.SetDataSize(output_size);
}

/*----------------------------------------------------------------------
|   EncryptingStream
+---------------------------------------------------------------------*/
class EncryptingStream : public AP4_ByteStream {
public:
    static AP4_Result Create(const AP4_UI08 *key, const AP4_UI08 *iv, AP4_ByteStream *output,
                             EncryptingStream *&stream);

    virtual AP4_Result ReadPartial(void *, AP4_Size, AP4_Size &) {
        return AP4_ERROR_NOT_SUPPORTED;
    }

    virtual AP4_Result WritePartial(const void *buffer,
                                    AP4_Size bytes_to_write,
                                    AP4_Size &bytes_written) {
        AP4_UI08 *out = new AP4_UI08[bytes_to_write + 16];
        AP4_Size out_size = bytes_to_write + 16;
        AP4_Result result = m_StreamCipher->ProcessBuffer((const AP4_UI08 *) buffer,
                                                          bytes_to_write,
                                                          out,
                                                          &out_size);
        if (AP4_SUCCEEDED(result)) {
            result = m_Output->Write(out, out_size);
            bytes_written = bytes_to_write;
            m_Size += bytes_to_write;
        } else {
            bytes_written = 0;
        }
        delete[] out;
        return result;
    }

    virtual AP4_Result Flush() {
        AP4_UI08 trailer[16];
        AP4_Size trailer_size = sizeof(trailer);
        AP4_Result result = m_StreamCipher->ProcessBuffer(NULL, 0, trailer, &trailer_size, true);
        if (AP4_SUCCEEDED(result) && trailer_size) {
            m_Output->Write(trailer, trailer_size);
            m_Size += 16 - (m_Size % 16);
        }

        return AP4_SUCCESS;
    }

    virtual AP4_Result Seek(AP4_Position) { return AP4_ERROR_NOT_SUPPORTED; }

    virtual AP4_Result Tell(AP4_Position &position) {
        position = m_Size;
        return AP4_SUCCESS;
    }

    virtual AP4_Result GetSize(AP4_LargeSize &size) {
        size = m_Size;
        return AP4_SUCCESS;
    }

    void AddReference() {
        ++m_ReferenceCount;
    }

    void Release() {
        if (--m_ReferenceCount == 0) {
            delete this;
        }
    }

private:
    EncryptingStream(AP4_CbcStreamCipher *stream_cipher, AP4_ByteStream *output) :
            m_ReferenceCount(1),
            m_StreamCipher(stream_cipher),
            m_Output(output),
            m_Size(0) {
        output->AddReference();
    }

    ~EncryptingStream() {
        m_Output->Release();
        delete m_StreamCipher;
    }

    unsigned int m_ReferenceCount;
    AP4_CbcStreamCipher *m_StreamCipher;
    AP4_ByteStream *m_Output;
    AP4_LargeSize m_Size;
};

/*----------------------------------------------------------------------
|   EncryptingStream::Create
+---------------------------------------------------------------------*/
AP4_Result
EncryptingStream::Create(const AP4_UI08 *key, const AP4_UI08 *iv, AP4_ByteStream *output,
                         EncryptingStream *&stream) {
    stream = NULL;
    AP4_BlockCipher *block_cipher = NULL;
    AP4_Result result = AP4_DefaultBlockCipherFactory::Instance.CreateCipher(
            AP4_BlockCipher::AES_128,
            AP4_BlockCipher::ENCRYPT,
            AP4_BlockCipher::CBC,
            NULL,
            key,
            16,
            block_cipher);
    if (AP4_FAILED(result)) return result;
    AP4_CbcStreamCipher *stream_cipher = new AP4_CbcStreamCipher(block_cipher);
    stream_cipher->SetIV(iv);
    stream = new EncryptingStream(stream_cipher, output);

    return AP4_SUCCESS;
}

/*----------------------------------------------------------------------
|   SampleEncrypter
+---------------------------------------------------------------------*/
class SampleEncrypter {
public:
    static AP4_Result Create(const AP4_UI08 *key, const AP4_UI08 *iv, SampleEncrypter *&encrypter);

    ~SampleEncrypter() {
        delete m_StreamCipher;
    }

    AP4_Result
    EncryptAudioSample(AP4_DataBuffer &sample, AP4_SampleDescription *sample_description);

    AP4_Result EncryptVideoSample(AP4_DataBuffer &sample, AP4_UI08 nalu_length_size);

private:
    SampleEncrypter(AP4_CbcStreamCipher *stream_cipher, const AP4_UI08 *iv) :
            m_StreamCipher(stream_cipher) {
        AP4_CopyMemory(m_IV, iv, 16);
    }

    AP4_CbcStreamCipher *m_StreamCipher;
    AP4_UI08 m_IV[16];
};

/*----------------------------------------------------------------------
|   SampleEncrypter::Create
+---------------------------------------------------------------------*/
AP4_Result
SampleEncrypter::Create(const AP4_UI08 *key, const AP4_UI08 *iv, SampleEncrypter *&encrypter) {
    encrypter = NULL;
    AP4_BlockCipher *block_cipher = NULL;
    AP4_Result result = AP4_DefaultBlockCipherFactory::Instance.CreateCipher(
            AP4_BlockCipher::AES_128,
            AP4_BlockCipher::ENCRYPT,
            AP4_BlockCipher::CBC,
            NULL,
            key,
            16,
            block_cipher);
    if (AP4_FAILED(result)) return result;
    AP4_CbcStreamCipher *stream_cipher = new AP4_CbcStreamCipher(block_cipher);
    encrypter = new SampleEncrypter(stream_cipher, iv);

    return AP4_SUCCESS;
}

/*----------------------------------------------------------------------
|   SampleEncrypter::EncryptAudioSample
|
|   AAC: entire frame
|   ADTS_header                        // 7 or 9
|   unencrypted_leader                 // 16 bytes
|   while (bytes_remaining() >= 16) {
|       protected_block                // 16 bytes
|   }
|   unencrypted_trailer                // 0-15 bytes
|
|   AC3: entire frame
|   unencrypted_leader                 // 16 bytes
|   while (bytes_remaining() >= 16) {
|       encrypted_block                // 16 bytes
|   }
|   unencrypted_trailer                // 0-15 bytes
|
|   E-AC3: each syncframe
|   unencrypted_leader                 // 16 bytes
|   while (bytes_remaining() >= 16) {
|       encrypted_block                // 16 bytes
|   }
|   unencrypted_trailer                // 0-15 bytes
|
+---------------------------------------------------------------------*/
AP4_Result
SampleEncrypter::EncryptAudioSample(AP4_DataBuffer &sample,
                                    AP4_SampleDescription *sample_description) {
    if (sample.GetDataSize() <= 16) {
        return AP4_SUCCESS;
    }

    // deal with E-AC3 syncframes
    if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_EC_3) {
        // syncword    : 16
        // strmtyp     : 2
        // substreamid : 3
        // frmsiz      : 11
        AP4_UI08 *data = sample.UseData();
        unsigned int data_size = sample.GetDataSize();
        while (data_size > 4) {
            unsigned int syncword = (data[0] << 8) | data[1];
            if (syncword != 0x0b77) {
                return AP4_ERROR_INVALID_FORMAT; // unexpected!
            }
            unsigned int frmsiz = 1 + (((data[2] << 8) | (data[3])) & 0x7FF);
            unsigned int frame_size = 2 * frmsiz;
            if (data_size < frame_size) {
                return AP4_ERROR_INVALID_FORMAT; // unexpected!
            }

            // encrypt the syncframe
            if (frame_size > 16) {
                unsigned int encrypted_block_count = (frame_size - 16) / 16;
                AP4_Size encrypted_size = encrypted_block_count * 16;
                m_StreamCipher->SetIV(m_IV);
                m_StreamCipher->ProcessBuffer(data + 16, encrypted_size, data + 16,
                                              &encrypted_size);
            }

            data += frame_size;
            data_size -= frame_size;
        }
    } else {
        unsigned int encrypted_block_count = (sample.GetDataSize() - 16) / 16;
        AP4_Size encrypted_size = encrypted_block_count * 16;
        m_StreamCipher->SetIV(m_IV);
        m_StreamCipher->ProcessBuffer(sample.UseData() + 16, encrypted_size, sample.UseData() + 16,
                                      &encrypted_size);
    }

    return AP4_SUCCESS;
}

/*----------------------------------------------------------------------
|   SampleEncrypter::EncryptVideoSample
|
|   Sequence of NAL Units:
|   NAL_unit_type_byte                // 1 byte
|   unencrypted_leader                // 31 bytes
|   while (bytes_remaining() > 16) {
|       protected_block_one_in_ten    // 16 bytes
|   }
|   unencrypted_trailer               // 1-16 bytes
+---------------------------------------------------------------------*/
AP4_Result
SampleEncrypter::EncryptVideoSample(AP4_DataBuffer &sample, AP4_UI08 nalu_length_size) {
    AP4_DataBuffer encrypted;

    AP4_UI08 *nalu = sample.UseData();
    AP4_Size bytes_remaining = sample.GetDataSize();
    while (bytes_remaining > nalu_length_size) {
        AP4_Size nalu_length = 0;
        switch (nalu_length_size) {
            case 1:
                nalu_length = nalu[0];
                break;

            case 2:
                nalu_length = AP4_BytesToUInt16BE(nalu);
                break;

            case 4:
                nalu_length = AP4_BytesToUInt32BE(nalu);
                break;

            default:
                break;
        }

        if (bytes_remaining < nalu_length_size + nalu_length) {
            break;
        }

        AP4_UI08 nalu_type = nalu[nalu_length_size] & 0x1F;
        if (nalu_length > 48 && (nalu_type == 1 || nalu_type == 5)) {
            AP4_Size encrypted_size = 16 * ((nalu_length - 32) / 16);
            if ((nalu_length % 16) == 0) {
                encrypted_size -= 16;
            }
            m_StreamCipher->SetIV(m_IV);
            for (unsigned int i = 0; i < encrypted_size; i += 10 * 16) {
                AP4_Size one_block_size = 16;
                m_StreamCipher->ProcessBuffer(nalu + nalu_length_size + 32 + i, one_block_size,
                                              nalu + nalu_length_size + 32 + i, &one_block_size);
            }

            // perform startcode emulation prevention
            AP4_DataBuffer escaped_nalu;
            PreventStartCodeEmulation(nalu + nalu_length_size, nalu_length, escaped_nalu);

            // the size may have changed
            // TODO: this could overflow if nalu_length_size is too small
            switch (nalu_length_size) {
                case 1:
                    nalu[0] = (AP4_UI08) (escaped_nalu.GetDataSize() & 0xFF);
                    break;

                case 2:
                    AP4_BytesFromUInt16BE(nalu, escaped_nalu.GetDataSize());
                    break;

                case 4:
                    AP4_BytesFromUInt32BE(nalu, escaped_nalu.GetDataSize());
                    break;

                default:
                    break;
            }

            encrypted.AppendData(nalu, nalu_length_size);
            encrypted.AppendData(escaped_nalu.GetData(), escaped_nalu.GetDataSize());
        } else {
            encrypted.AppendData(nalu, nalu_length_size);
            encrypted.AppendData(nalu + nalu_length_size, nalu_length);
        }

        nalu += nalu_length_size + nalu_length;
        bytes_remaining -= nalu_length_size + nalu_length;
    }

    sample.SetData(encrypted.GetData(), encrypted.GetDataSize());

    return AP4_SUCCESS;
}

/*----------------------------------------------------------------------
|   GetSamplingFrequencyIndex
+---------------------------------------------------------------------*/
static unsigned int
GetSamplingFrequencyIndex(unsigned int sampling_frequency) {
    switch (sampling_frequency) {
        case 96000:
            return 0;
        case 88200:
            return 1;
        case 64000:
            return 2;
        case 48000:
            return 3;
        case 44100:
            return 4;
        case 32000:
            return 5;
        case 24000:
            return 6;
        case 22050:
            return 7;
        case 16000:
            return 8;
        case 12000:
            return 9;
        case 11025:
            return 10;
        case 8000:
            return 11;
        case 7350:
            return 12;
        default:
            return 0;
    }
}

/*----------------------------------------------------------------------
|   WriteAdtsHeader
+---------------------------------------------------------------------*/
static AP4_Result
WriteAdtsHeader(AP4_ByteStream &output,
                unsigned int frame_size,
                unsigned int sampling_frequency_index,
                unsigned int channel_configuration) {
    unsigned char bits[7];

    bits[0] = 0xFF;
    bits[1] = 0xF1; // 0xF9 (MPEG2)
    bits[2] = 0x40 | (sampling_frequency_index << 2) | (channel_configuration >> 2);
    bits[3] = ((channel_configuration & 0x3) << 6) | ((frame_size + 7) >> 11);
    bits[4] = ((frame_size + 7) >> 3) & 0xFF;
    bits[5] = (((frame_size + 7) << 5) & 0xFF) | 0x1F;
    bits[6] = 0xFC;

    return output.Write(bits, 7);

    /*
        0:  syncword 12 always: '111111111111' 
        12: ID 1 0: MPEG-4, 1: MPEG-2 
        13: layer 2 always: '00' 
        15: protection_absent 1  
        16: profile 2  
        18: sampling_frequency_index 4  
        22: private_bit 1  
        23: channel_configuration 3  
        26: original/copy 1  
        27: home 1  
        28: emphasis 2 only if ID == 0 

        ADTS Variable header: these can change from frame to frame 
        28: copyright_identification_bit 1  
        29: copyright_identification_start 1  
        30: aac_frame_length 13 length of the frame including header (in bytes) 
        43: adts_buffer_fullness 11 0x7FF indicates VBR 
        54: no_raw_data_blocks_in_frame 2  
        ADTS Error check 
        crc_check 16 only if protection_absent == 0 
   */
}

/*----------------------------------------------------------------------
|   WriteAc4Header
+---------------------------------------------------------------------*/
static AP4_Result
WriteAc4Header(AP4_ByteStream &output,
               unsigned int frame_size) {
    unsigned char bits[7];

    bits[0] = 0xac;
    bits[1] = 0x40;
    bits[2] = 0xff;
    bits[3] = 0xff;
    bits[4] = (frame_size >> 16) & 0xFF;
    bits[5] = (frame_size >> 8) & 0xFF;
    bits[6] = (frame_size) & 0xFF;

    return output.Write(bits, 7);
}

/*----------------------------------------------------------------------
|   MakeAudioSetupData
+---------------------------------------------------------------------*/
static AP4_Result
MakeAudioSetupData(AP4_DataBuffer &audio_setup_data,
                   AP4_SampleDescription *sample_description,
                   AP4_DataBuffer &sample_data /* needed for encrypted AC3 */) {
    AP4_UI08 audio_type[4];
    AP4_DataBuffer setup_data;

    if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_MP4A) {
        AP4_MpegAudioSampleDescription *mpeg_audio_desc = AP4_DYNAMIC_CAST(
                AP4_MpegAudioSampleDescription, sample_description);
        if (mpeg_audio_desc == NULL ||
            !(mpeg_audio_desc->GetObjectTypeId() == AP4_OTI_MPEG4_AUDIO ||
              mpeg_audio_desc->GetObjectTypeId() == AP4_OTI_MPEG2_AAC_AUDIO_LC ||
              mpeg_audio_desc->GetObjectTypeId() == AP4_OTI_MPEG2_AAC_AUDIO_MAIN)) {
            return AP4_ERROR_NOT_SUPPORTED;
        }
        const AP4_DataBuffer &dsi = mpeg_audio_desc->GetDecoderInfo();
        setup_data.SetData(dsi.GetData(), dsi.GetDataSize());

        AP4_Mp4AudioDecoderConfig dec_config;
        AP4_Result result = dec_config.Parse(dsi.GetData(), dsi.GetDataSize());
        if (AP4_FAILED(result)) {
            return AP4_ERROR_INVALID_FORMAT;
        }

        if (dec_config.m_Extension.m_SbrPresent || dec_config.m_Extension.m_PsPresent) {
            if (dec_config.m_Extension.m_PsPresent) {
                audio_type[0] = 'z';
                audio_type[1] = 'a';
                audio_type[2] = 'c';
                audio_type[3] = 'p';
            } else {
                audio_type[0] = 'z';
                audio_type[1] = 'a';
                audio_type[2] = 'c';
                audio_type[3] = 'h';
            }
        } else {
            audio_type[0] = 'z';
            audio_type[1] = 'a';
            audio_type[2] = 'a';
            audio_type[3] = 'c';
        }
    } else if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_AC_3) {
        audio_type[0] = 'z';
        audio_type[1] = 'a';
        audio_type[2] = 'c';
        audio_type[3] = '3';

        // the setup data is the first 10 bytes of a syncframe
        if (sample_data.GetDataSize() >= 10) {
            setup_data.SetData(sample_data.GetData(), 10);
        }
    } else if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_EC_3) {
        audio_type[0] = 'z';
        audio_type[1] = 'e';
        audio_type[2] = 'c';
        audio_type[3] = '3';

        AP4_Dec3Atom *dec3 = AP4_DYNAMIC_CAST(AP4_Dec3Atom,
                                              sample_description->GetDetails().GetChild(
                                                      AP4_ATOM_TYPE_DEC3));
        if (dec3 == NULL) {
            fprintf(stderr, "ERROR: failed to find 'dec3' atom in sample description\n");
            return 1;
        }
        setup_data.SetData(dec3->GetRawBytes().GetData(), dec3->GetRawBytes().GetDataSize());
    } else {
        return AP4_SUCCESS;
    }

    audio_setup_data.SetDataSize(8 + setup_data.GetDataSize());
    AP4_UI08 *payload = audio_setup_data.UseData();
    payload[0] = audio_type[0];
    payload[1] = audio_type[1];
    payload[2] = audio_type[2];
    payload[3] = audio_type[3];
    payload[4] = 0; // priming
    payload[5] = 0; // priming
    payload[6] = 1; // version
    payload[7] = setup_data.GetDataSize(); // setup_data_length
    if (setup_data.GetDataSize()) {
        AP4_CopyMemory(&payload[8], setup_data.GetData(), setup_data.GetDataSize());
    }

    return AP4_SUCCESS;
}

/*----------------------------------------------------------------------
|   MakeSampleAesAudioDescriptor
+---------------------------------------------------------------------*/
static AP4_Result
MakeSampleAesAudioDescriptor(AP4_DataBuffer &descriptor,
                             AP4_SampleDescription *sample_description,
                             AP4_DataBuffer &sample_data /* needed for encrypted AC3 */) {
    // descriptor
    descriptor.SetDataSize(6);
    AP4_UI08 *payload = descriptor.UseData();
    payload[0] = AP4_MPEG2_PRIVATE_DATA_INDICATOR_DESCRIPTOR_TAG;
    payload[1] = 4;
    if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_MP4A) {
        payload[2] = 'a';
        payload[3] = 'a';
        payload[4] = 'c';
        payload[5] = 'd';
    } else if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_AC_3) {
        payload[2] = 'a';
        payload[3] = 'c';
        payload[4] = '3';
        payload[5] = 'd';
    } else if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_EC_3) {
        payload[2] = 'e';
        payload[3] = 'c';
        payload[4] = '3';
        payload[5] = 'd';
    }

    // compute the audio setup data
    AP4_DataBuffer audio_setup_data;
    AP4_Result result = MakeAudioSetupData(audio_setup_data, sample_description, sample_data);
    if (AP4_FAILED(result)) {
        fprintf(stderr, "ERROR: failed to make audio setup data (%d)\n", result);
        return result;
    }

    descriptor.SetDataSize(descriptor.GetDataSize() + 6 + audio_setup_data.GetDataSize());
    payload = descriptor.UseData() + 6;
    payload[0] = AP4_MPEG2_REGISTRATION_DESCRIPTOR_TAG;
    payload[1] = 4 + audio_setup_data.GetDataSize();
    payload[2] = 'a';
    payload[3] = 'p';
    payload[4] = 'a';
    payload[5] = 'd';
    AP4_CopyMemory(&payload[6], audio_setup_data.GetData(), audio_setup_data.GetDataSize());

    return AP4_SUCCESS;
}

/*----------------------------------------------------------------------
|   MakeSampleAesVideoDescriptor
+---------------------------------------------------------------------*/
static AP4_Result
MakeSampleAesVideoDescriptor(AP4_DataBuffer &descriptor) {
    descriptor.SetDataSize(6);
    AP4_UI08 *payload = descriptor.UseData();
    payload[0] = AP4_MPEG2_PRIVATE_DATA_INDICATOR_DESCRIPTOR_TAG;
    payload[1] = 4;
    payload[2] = 'z';
    payload[3] = 'a';
    payload[4] = 'v';
    payload[5] = 'c';

    return AP4_SUCCESS;
}

/*----------------------------------------------------------------------
|   PackedAudioWriter
+---------------------------------------------------------------------*/
class PackedAudioWriter {
public:
    AP4_Result WriteHeader(double timestamp,
                           const char *private_extension_name,
                           const AP4_UI08 *private_extension_data,
                           unsigned int private_extension_data_size,
                           AP4_ByteStream &output);

    AP4_Result WriteSample(AP4_Sample &sample,
                           AP4_DataBuffer &sample_data,
                           AP4_SampleDescription *sample_description,
                           AP4_ByteStream &output);

};

/*----------------------------------------------------------------------
|   PackedAudioWriter::WriteHeader
+---------------------------------------------------------------------*/
AP4_Result
PackedAudioWriter::WriteHeader(double timestamp,
                               const char *private_extension_name,
                               const AP4_UI08 *private_extension_data,
                               unsigned int private_extension_data_size,
                               AP4_ByteStream &output) {
    unsigned int header_size = 10 + 10 + 45 + 8;
    unsigned int private_extension_name_size = private_extension_name ?
                                               (unsigned int) AP4_StringLength(
                                                       private_extension_name) + 1 : 0;
    if (private_extension_name) {
        header_size += 10 + private_extension_name_size + private_extension_data_size;;
    }
    AP4_UI08 *header = new AP4_UI08[header_size];

    /* ID3 Tag Header
     ID3v2/file identifier      "ID3"
     ID3v2 version              $04 00
     ID3v2 flags                %abcd0000
     ID3v2 size                 4 * %0xxxxxxx
    */
    header[0] = 'I';
    header[1] = 'D';
    header[2] = '3';
    header[3] = 4;
    header[4] = 0;
    header[5] = 0;
    header[6] = 0;
    header[7] = 0;
    header[8] = ((header_size - 10) >> 7) & 0x7F;
    header[9] = (header_size - 10) & 0x7F;

    /* PRIV frame
     Frame ID   $xx xx xx xx  (four characters)
     Size       $xx xx xx xx
     Flags      $xx xx
     
     Owner identifier      <text string> $00
     The private data      <binary data>    
    */
    header[10] = 'P';
    header[11] = 'R';
    header[12] = 'I';
    header[13] = 'V';
    header[14] = 0;
    header[15] = 0;
    header[16] = 0;
    header[17] = 45 + 8;
    header[18] = 0;
    header[19] = 0;
    AP4_CopyMemory(&header[20], "com.apple.streaming.transportStreamTimestamp", 45);
    AP4_UI64 mpeg_ts = (AP4_UI64) (timestamp * 90000.0);
    AP4_BytesFromUInt64BE(&header[10 + 10 + 45], mpeg_ts);

    // add the extra private extension if needed
    if (private_extension_name) {
        AP4_UI08 *ext = &header[10 + 10 + 45 + 8];
        ext[0] = 'P';
        ext[1] = 'R';
        ext[2] = 'I';
        ext[3] = 'V';
        ext[4] = 0;
        ext[5] = 0;
        ext[6] = ((private_extension_name_size + private_extension_data_size) >> 7) & 0x7F;
        ext[7] = (private_extension_name_size + private_extension_data_size) & 0x7F;
        ext[8] = 0;
        ext[9] = 0;
        AP4_CopyMemory(&ext[10], private_extension_name, private_extension_name_size);
        AP4_CopyMemory(&ext[10 + private_extension_name_size], private_extension_data,
                       private_extension_data_size);
    }

    // write the header out
    AP4_Result result = output.Write(header, header_size);
    delete[] header;

    return result;
}

/*----------------------------------------------------------------------
|   PackedAudioWriter::WriteSample
+---------------------------------------------------------------------*/
AP4_Result
PackedAudioWriter::WriteSample(AP4_Sample &sample,
                               AP4_DataBuffer &sample_data,
                               AP4_SampleDescription *sample_description,
                               AP4_ByteStream &output) {
    AP4_AudioSampleDescription *audio_desc = AP4_DYNAMIC_CAST(AP4_AudioSampleDescription,
                                                              sample_description);
    if (audio_desc == NULL) {
        return AP4_ERROR_INVALID_FORMAT;
    }
    if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_MP4A) {
        AP4_MpegAudioSampleDescription *mpeg_audio_desc = AP4_DYNAMIC_CAST(
                AP4_MpegAudioSampleDescription, sample_description);

        if (mpeg_audio_desc == NULL) return AP4_ERROR_NOT_SUPPORTED;
        if (mpeg_audio_desc->GetMpeg4AudioObjectType() != AP4_MPEG4_AUDIO_OBJECT_TYPE_AAC_LC &&
            mpeg_audio_desc->GetMpeg4AudioObjectType() != AP4_MPEG4_AUDIO_OBJECT_TYPE_AAC_MAIN &&
            mpeg_audio_desc->GetMpeg4AudioObjectType() != AP4_MPEG4_AUDIO_OBJECT_TYPE_SBR &&
            mpeg_audio_desc->GetMpeg4AudioObjectType() != AP4_MPEG4_AUDIO_OBJECT_TYPE_PS) {
            return AP4_ERROR_NOT_SUPPORTED;
        }

        unsigned int sample_rate = mpeg_audio_desc->GetSampleRate();
        unsigned int channel_count = mpeg_audio_desc->GetChannelCount();
        const AP4_DataBuffer &dsi = mpeg_audio_desc->GetDecoderInfo();
        if (dsi.GetDataSize()) {
            AP4_Mp4AudioDecoderConfig dec_config;
            AP4_Result result = dec_config.Parse(dsi.GetData(), dsi.GetDataSize());
            if (AP4_SUCCEEDED(result)) {
                sample_rate = dec_config.m_SamplingFrequency;
                channel_count = dec_config.m_ChannelCount;
            }
        }
        unsigned int sampling_frequency_index = GetSamplingFrequencyIndex(sample_rate);
        unsigned int channel_configuration = channel_count;

        WriteAdtsHeader(output, sample.GetSize(), sampling_frequency_index, channel_configuration);
    } else if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_AC_4) {
        WriteAc4Header(output, sample.GetSize());
    }
    return output.Write(sample_data.GetData(), sample_data.GetDataSize());
}

/*----------------------------------------------------------------------
|   ReadSample
+---------------------------------------------------------------------*/
static AP4_Result
ReadSample(SampleReader &reader,
           AP4_Track &track,
           AP4_Sample &sample,
           AP4_DataBuffer &sample_data,
           double &ts,
           double &duration,
           bool &eos) {
    AP4_Result result = reader.ReadSample(sample, sample_data);
    if (AP4_FAILED(result)) {
        if (result == AP4_ERROR_EOS) {
            // advance the timestamp by the last sample's duration
            ts += duration;
            eos = true;
            return AP4_SUCCESS;
        } else {
            return result;
        }
    }
    ts = (double) sample.GetDts() / (double) track.GetMediaTimeScale();
    duration = sample.GetDuration() / (double) track.GetMediaTimeScale();

    return AP4_SUCCESS;
}

/*----------------------------------------------------------------------
|   WriteSamples
+---------------------------------------------------------------------*/
static AP4_Result
WriteSamples(AP4_Mpeg2TsWriter *ts_writer,
             PackedAudioWriter *packed_writer,
             AP4_Track *audio_track,
             SampleReader *audio_reader,
             AP4_Mpeg2TsWriter::SampleStream *audio_stream,
             AP4_Track *video_track,
             SampleReader *video_reader,
             AP4_Mpeg2TsWriter::SampleStream *video_stream,
             unsigned int segment_duration_threshold,
             AP4_UI08 nalu_length_size) {
    AP4_Sample audio_sample;
    AP4_DataBuffer audio_sample_data;
    unsigned int audio_sample_count = 0;
    double audio_ts = 0.0;
    double audio_frame_duration = 0.0;
    bool audio_eos = false;
    AP4_Sample video_sample;
    AP4_DataBuffer video_sample_data;
    unsigned int video_sample_count = 0;
    double video_ts = 0.0;
    double video_frame_duration = 0.0;
    bool video_eos = false;
    double last_ts = 0.0;
    unsigned int segment_number = 0;
    AP4_ByteStream *segment_output = NULL;
    double segment_duration = 0.0;
    AP4_Array<double> segment_durations;
    AP4_Array<AP4_UI32> segment_sizes;
    AP4_Position segment_position = 0;
    AP4_Array<AP4_Position> segment_positions;
    AP4_Array<AP4_Position> iframe_positions;
    AP4_Array<AP4_UI32> iframe_sizes;
    AP4_Array<double> iframe_times;
    AP4_Array<double> iframe_durations;
    AP4_Array<AP4_UI32> iframe_segment_indexes;
    bool new_segment = true;
    AP4_ByteStream *raw_output = NULL;
    AP4_ByteStream *playlist = NULL;
    char string_buffer[4096];
    SampleEncrypter *sample_encrypter = NULL;
    AP4_Result result = AP4_SUCCESS;

    char msg[256];

    // prime the samples
    if (audio_reader) {
        result = ReadSample(*audio_reader, *audio_track, audio_sample, audio_sample_data, audio_ts,
                            audio_frame_duration, audio_eos);
        __android_log_print(ANDROID_LOG_DEBUG, "TAG", "11111");
        if (AP4_FAILED(result)) { return result; }
    }
    if (video_reader) {
        result = ReadSample(*video_reader, *video_track, video_sample, video_sample_data, video_ts,
                            video_frame_duration, video_eos);
        if (AP4_FAILED(result)) {
            __android_log_print(ANDROID_LOG_DEBUG, "TAG", "2222");
            return result;
        }
    }

    for (;;) {
        bool sync_sample = false;
        AP4_Track *chosen_track = NULL;
        if (audio_track && !audio_eos) {
            chosen_track = audio_track;
            if (video_track == NULL) {
                if (audio_track->GetSampleDescription(0)->GetFormat() == AP4_SAMPLE_FORMAT_AC_4) {
                    if (audio_sample.IsSync()) {
                        sync_sample = true;
                    }
                } else {
                    sync_sample = true;
                }
            }
        }
        if (video_track && !video_eos) {
            if (audio_track) {
                if (video_ts <= audio_ts) {
                    chosen_track = video_track;
                }
            } else {
                chosen_track = video_track;
            }
            if (chosen_track == video_track && video_sample.IsSync()) {
                sync_sample = true;
            }
        }

        // check if we need to start a new segment
        if (HlsOptions.segment_duration && (sync_sample || chosen_track == NULL)) {
            if (video_track) {
                segment_duration = video_ts - last_ts;
            } else {
                segment_duration = audio_ts - last_ts;
            }
            if ((segment_duration >= (double) HlsOptions.segment_duration -
                                     (double) segment_duration_threshold / 1000.0) ||
                chosen_track == NULL) {
                if (video_track) {
                    last_ts = video_ts;
                } else {
                    last_ts = audio_ts;
                }
                if (segment_output) {
                    // flush the output stream
                    segment_output->Flush();

                    // compute the segment size (including padding)
                    AP4_Position segment_end = 0;
                    segment_output->Tell(segment_end);
                    AP4_UI32 segment_size = 0;
                    if (HlsOptions.encryption_mode == ENCRYPTION_MODE_AES_128) {
                        segment_size = (AP4_UI32) segment_end;
                    } else if (segment_end > segment_position) {
                        segment_size = (AP4_UI32) (segment_end - segment_position);
                    }

                    // update counters
                    segment_sizes.Append(segment_size);
                    segment_positions.Append(segment_position);
                    segment_durations.Append(segment_duration);

                    if (segment_duration != 0.0) {
                        double segment_bitrate = 8.0 * (double) segment_size / segment_duration;
                        if (segment_bitrate > Stats.max_segment_bitrate) {
                            Stats.max_segment_bitrate = segment_bitrate;
                        }
                    }
                    if (HlsOptions.verbose) {
                        printf("Segment %d, duration=%.2f, %d audio samples, %d video samples, %d bytes @%lld\n",
                               segment_number,
                               segment_duration,
                               audio_sample_count,
                               video_sample_count,
                               segment_size,
                               segment_position);
                    }
                    if (!HlsOptions.output_single_file) {
                        segment_output->Release();
                        segment_output = NULL;
                    }
                    ++segment_number;
                    audio_sample_count = 0;
                    video_sample_count = 0;
                }
                new_segment = true;
            }
        }

        // check if we're done
        if (chosen_track == NULL) break;

        if (new_segment) {
            new_segment = false;

            // compute the new segment position
            segment_position = 0;
            if (HlsOptions.output_single_file) {
                if (raw_output) {
                    raw_output->Tell(segment_position);
                }
            }

            // manage the new segment stream
            if (segment_output == NULL) {
                segment_output = OpenOutput(HlsOptions.segment_filename_template, segment_number);
                raw_output = segment_output;
                if (segment_output == NULL) {
                    return AP4_ERROR_CANNOT_OPEN_FILE;
                }
            }
            if (HlsOptions.encryption_mode != ENCRYPTION_MODE_NONE) {
                if (HlsOptions.encryption_iv_mode == ENCRYPTION_IV_MODE_SEQUENCE) {
                    AP4_SetMemory(HlsOptions.encryption_iv, 0, sizeof(HlsOptions.encryption_iv));
                    AP4_BytesFromUInt32BE(&HlsOptions.encryption_iv[12], segment_number);
                }
            }
            if (HlsOptions.encryption_mode == ENCRYPTION_MODE_AES_128) {
                EncryptingStream *encrypting_stream = NULL;
                result = EncryptingStream::Create(HlsOptions.encryption_key,
                                                  HlsOptions.encryption_iv,
                                                  raw_output, encrypting_stream);
                if (AP4_FAILED(result)) {
                    fprintf(stderr, "ERROR: failed to create encrypting stream (%d)\n", result);
                    return result;
                }
                segment_output->Release();
                segment_output = encrypting_stream;
            } else if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES) {
                delete sample_encrypter;
                sample_encrypter = NULL;
                result = SampleEncrypter::Create(HlsOptions.encryption_key,
                                                 HlsOptions.encryption_iv,
                                                 sample_encrypter);
                if (AP4_FAILED(result)) {
                    sprintf(msg, "ERROR: failed to create sample encrypter (%d)\n", result);
                    __android_log_print(ANDROID_LOG_DEBUG, "TAG", "3333:%s", msg);
                    return result;
                }
            }

            // write the PAT and PMT
            if (ts_writer) {
                // update the descriptors if needed
                if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES) {
                    AP4_DataBuffer descriptor;
                    if (audio_track) {
                        result = MakeSampleAesAudioDescriptor(descriptor,
                                                              audio_track->GetSampleDescription(0),
                                                              audio_sample_data);
                        if (AP4_SUCCEEDED(result) && descriptor.GetDataSize()) {
                            audio_stream->SetDescriptor(descriptor.GetData(),
                                                        descriptor.GetDataSize());
                        } else {
                            sprintf(msg, "ERROR: failed to create sample-aes descriptor (%d)\n",
                                    result);
                            __android_log_print(ANDROID_LOG_DEBUG, "TAG", "4444:%s", msg);
                            return result;
                        }
                    }
                    if (video_track) {
                        result = MakeSampleAesVideoDescriptor(descriptor);
                        if (AP4_SUCCEEDED(result) && descriptor.GetDataSize()) {
                            video_stream->SetDescriptor(descriptor.GetData(),
                                                        descriptor.GetDataSize());
                        } else {
                            sprintf(msg, "ERROR: failed to create sample-aes descriptor (%d)\n",
                                    result);
                            __android_log_print(ANDROID_LOG_DEBUG, "TAG", "555:%s", msg);
                            return result;
                        }
                    }
                }

                ts_writer->WritePAT(*segment_output);
                ts_writer->WritePMT(*segment_output);
            } else if (packed_writer && chosen_track == audio_track) {
                AP4_DataBuffer private_extension_buffer;
                const char *private_extension_name = NULL;
                const unsigned char *private_extension_data = NULL;
                unsigned int private_extension_data_size = 0;
                if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES) {
                    private_extension_name = "com.apple.streaming.audioDescription";
                    result = MakeAudioSetupData(private_extension_buffer,
                                                audio_track->GetSampleDescription(0),
                                                audio_sample_data);
                    if (AP4_FAILED(result)) {
                        sprintf(msg, "ERROR: failed to make audio setup data (%d)\n", result);
                        __android_log_print(ANDROID_LOG_DEBUG, "TAG", "6566:%s", msg);
                        return 1;
                    }
                    private_extension_data = private_extension_buffer.GetData();
                    private_extension_data_size = private_extension_buffer.GetDataSize();
                }
                packed_writer->WriteHeader(audio_ts,
                                           private_extension_name,
                                           private_extension_data,
                                           private_extension_data_size,
                                           *segment_output);
            }
        }

        // write the samples out and advance to the next sample
        if (chosen_track == audio_track) {
            // perform sample-level encryption if needed
            if (sample_encrypter) {
                result = sample_encrypter->EncryptAudioSample(audio_sample_data,
                                                              audio_track->GetSampleDescription(
                                                                      audio_sample.GetDescriptionIndex()));
                if (AP4_FAILED(result)) {
                    sprintf(msg, "ERROR: failed to encrypt audio sample (%d)\n", result);
                    __android_log_print(ANDROID_LOG_DEBUG, "TAG", "7777:%s", msg);
                    return result;
                }
            }

            // write the sample data
            if (audio_stream) {
                result = audio_stream->WriteSample(audio_sample,
                                                   audio_sample_data,
                                                   audio_track->GetSampleDescription(
                                                           audio_sample.GetDescriptionIndex()),
                                                   video_track == NULL,
                                                   *segment_output);
            } else if (packed_writer) {
                result = packed_writer->WriteSample(audio_sample,
                                                    audio_sample_data,
                                                    audio_track->GetSampleDescription(
                                                            audio_sample.GetDescriptionIndex()),
                                                    *segment_output);
            } else {
                return AP4_ERROR_INTERNAL;
            }
            if (AP4_FAILED(result)) return result;

            result = ReadSample(*audio_reader, *audio_track, audio_sample, audio_sample_data,
                                audio_ts, audio_frame_duration, audio_eos);
            if (AP4_FAILED(result)) {
                __android_log_print(ANDROID_LOG_DEBUG, "TAG", "8888:%s", msg);
                return result;
            }
            ++audio_sample_count;
        } else if (chosen_track == video_track) {
            // perform sample-level encryption if needed
            if (sample_encrypter) {
                result = sample_encrypter->EncryptVideoSample(video_sample_data, nalu_length_size);
                if (AP4_FAILED(result)) {
                    sprintf(msg, "ERROR: failed to encrypt video sample (%d)\n", result);
                    __android_log_print(ANDROID_LOG_DEBUG, "TAG", "9999:%s", msg);
                    return result;
                }
            }

            // write the sample data
            AP4_Position frame_start = 0;
            segment_output->Tell(frame_start);
            result = video_stream->WriteSample(video_sample,
                                               video_sample_data,
                                               video_track->GetSampleDescription(
                                                       video_sample.GetDescriptionIndex()),
                                               true,
                                               *segment_output);
            if (AP4_FAILED(result)) return result;
            AP4_Position frame_end = 0;
            segment_output->Tell(frame_end);

            // measure I frames
            if (video_sample.IsSync()) {
                AP4_UI64 frame_size = 0;
                if (frame_end > frame_start) {
                    frame_size = frame_end - frame_start;
                }
                if (HlsOptions.encryption_mode == ENCRYPTION_MODE_AES_128) {
                    frame_start += segment_position;
                }
                iframe_positions.Append(frame_start);
                iframe_sizes.Append((AP4_UI32) frame_size);
                iframe_times.Append(video_ts);
                iframe_segment_indexes.Append(segment_number);
                iframe_durations.Append(0.0); // will be computed later
                if (HlsOptions.verbose) {
                    printf("I-Frame: %d@%lld, t=%f\n", (AP4_UI32) frame_size, frame_start,
                           video_ts);
                }
            }

            // read the next sample
            result = ReadSample(*video_reader, *video_track, video_sample, video_sample_data,
                                video_ts, video_frame_duration, video_eos);
            if (AP4_FAILED(result)) {
                __android_log_print(ANDROID_LOG_DEBUG, "TAG", "aaaa:%s", msg);
                return result;
            }
            ++video_sample_count;
        } else {
            break;
        }
    }

    // create the media playlist/index file
    playlist = OpenOutput(HlsOptions.index_filename, 0);
    if (playlist == NULL) {
        __android_log_print(ANDROID_LOG_DEBUG, "TAG", "打开文件失败");
        return AP4_ERROR_CANNOT_OPEN_FILE;
    }

    unsigned int target_duration = 0;
    double total_duration = 0.0;
    for (unsigned int i = 0; i < segment_durations.ItemCount(); i++) {
        if ((unsigned int) (segment_durations[i] + 0.5) > target_duration) {
            target_duration = (unsigned int) segment_durations[i];
        }
        total_duration += segment_durations[i];
    }

    playlist->WriteString("#EXTM3U\r\n");
    if (HlsOptions.hls_version > 1) {
        sprintf(string_buffer, "#EXT-X-VERSION:%d\r\n", HlsOptions.hls_version);
        playlist->WriteString(string_buffer);
    }
    playlist->WriteString("#EXT-X-PLAYLIST-TYPE:VOD\r\n");
    if (video_track) {
        playlist->WriteString("#EXT-X-INDEPENDENT-SEGMENTS\r\n");
    }
    if (HlsOptions.allow_cache) {
        playlist->WriteString("#EXT-X-ALLOW-CACHE:");
        playlist->WriteString(HlsOptions.allow_cache);
        playlist->WriteString("\r\n");
    }
    playlist->WriteString("#EXT-X-TARGETDURATION:");
    sprintf(string_buffer, "%d\r\n", target_duration);
    playlist->WriteString(string_buffer);
    playlist->WriteString("#EXT-X-MEDIA-SEQUENCE:0\r\n");

    if (HlsOptions.encryption_mode != ENCRYPTION_MODE_NONE) {
        if (HlsOptions.encryption_key_lines.ItemCount()) {
            for (unsigned int i = 0; i < HlsOptions.encryption_key_lines.ItemCount(); i++) {
                AP4_String &key_line = HlsOptions.encryption_key_lines[i];
                const char *key_line_cstr = key_line.GetChars();
                bool omit_iv = false;

                // omit the IV if the key line starts with a "!" (and skip the "!")
                if (key_line[0] == '!') {
                    ++key_line_cstr;
                    omit_iv = true;
                }

                playlist->WriteString("#EXT-X-KEY:METHOD=");
                if (HlsOptions.encryption_mode == ENCRYPTION_MODE_AES_128) {
                    playlist->WriteString("AES-128");
                } else if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES) {
                    playlist->WriteString("SAMPLE-AES");
                }
                playlist->WriteString(",");
                playlist->WriteString(key_line_cstr);
                if ((HlsOptions.encryption_iv_mode == ENCRYPTION_IV_MODE_RANDOM ||
                     HlsOptions.encryption_iv_mode == ENCRYPTION_IV_MODE_FPS) && !omit_iv) {
                    playlist->WriteString(",IV=0x");
                    char iv_hex[33];
                    iv_hex[32] = 0;
                    AP4_FormatHex(HlsOptions.encryption_iv, 16, iv_hex);
                    playlist->WriteString(iv_hex);
                }
                playlist->WriteString("\r\n");
            }
        } else {
            playlist->WriteString("#EXT-X-KEY:METHOD=");
            if (HlsOptions.encryption_mode == ENCRYPTION_MODE_AES_128) {
                playlist->WriteString("AES-128");
            } else if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES) {
                playlist->WriteString("SAMPLE-AES");
            }
            playlist->WriteString(",URI=\"");
            playlist->WriteString(HlsOptions.encryption_key_uri);
            playlist->WriteString("\"");
            if (HlsOptions.encryption_iv_mode == ENCRYPTION_IV_MODE_RANDOM) {
                playlist->WriteString(",IV=0x");
                char iv_hex[33];
                iv_hex[32] = 0;
                AP4_FormatHex(HlsOptions.encryption_iv, 16, iv_hex);
                playlist->WriteString(iv_hex);
            }
            if (HlsOptions.encryption_key_format) {
                playlist->WriteString(",KEYFORMAT=\"");
                playlist->WriteString(HlsOptions.encryption_key_format);
                playlist->WriteString("\"");
            }
            if (HlsOptions.encryption_key_format_versions) {
                playlist->WriteString(",KEYFORMATVERSIONS=\"");
                playlist->WriteString(HlsOptions.encryption_key_format_versions);
                playlist->WriteString("\"");
            }
            playlist->WriteString("\r\n");
        }
    }

    for (unsigned int i = 0; i < segment_durations.ItemCount(); i++) {
        if (HlsOptions.hls_version >= 3) {
            sprintf(string_buffer, "#EXTINF:%f,\r\n", segment_durations[i]);
        } else {
            sprintf(string_buffer, "#EXTINF:%u,\r\n", (unsigned int) (segment_durations[i] + 0.5));
        }
        playlist->WriteString(string_buffer);
        if (HlsOptions.output_single_file) {
            sprintf(string_buffer, "#EXT-X-BYTERANGE:%d@%lld\r\n", segment_sizes[i],
                    segment_positions[i]);
            playlist->WriteString(string_buffer);
        }
        sprintf(string_buffer, HlsOptions.segment_url_template, i);
        playlist->WriteString(string_buffer);
        playlist->WriteString("\r\n");
    }

    playlist->WriteString("#EXT-X-ENDLIST\r\n");
    playlist->Release();

    // create the iframe playlist/index file
    if (video_track && HlsOptions.hls_version >= 4) {
        // compute the iframe durations and target duration
        for (unsigned int i = 0; i < iframe_positions.ItemCount(); i++) {
            double iframe_duration = 0.0;
            if (i + 1 < iframe_positions.ItemCount()) {
                iframe_duration = iframe_times[i + 1] - iframe_times[i];
            } else if (total_duration > iframe_times[i]) {
                iframe_duration = total_duration - iframe_times[i];
            }
            iframe_durations[i] = iframe_duration;
        }
        unsigned int iframes_target_duration = 0;
        for (unsigned int i = 0; i < iframe_durations.ItemCount(); i++) {
            if ((unsigned int) (iframe_durations[i] + 0.5) > iframes_target_duration) {
                iframes_target_duration = (unsigned int) iframe_durations[i];
            }
        }

        playlist = OpenOutput(HlsOptions.iframe_index_filename, 0);
        if (playlist == NULL) return AP4_ERROR_CANNOT_OPEN_FILE;

        playlist->WriteString("#EXTM3U\r\n");
        if (HlsOptions.hls_version > 1) {
            sprintf(string_buffer, "#EXT-X-VERSION:%d\r\n", HlsOptions.hls_version);
            playlist->WriteString(string_buffer);
        }
        playlist->WriteString("#EXT-X-PLAYLIST-TYPE:VOD\r\n");
        playlist->WriteString("#EXT-X-I-FRAMES-ONLY\r\n");
        playlist->WriteString("#EXT-X-INDEPENDENT-SEGMENTS\r\n");
        playlist->WriteString("#EXT-X-TARGETDURATION:");
        sprintf(string_buffer, "%d\r\n", iframes_target_duration);
        playlist->WriteString(string_buffer);
        playlist->WriteString("#EXT-X-MEDIA-SEQUENCE:0\r\n");

        if (HlsOptions.encryption_mode != ENCRYPTION_MODE_NONE) {
            playlist->WriteString("#EXT-X-KEY:METHOD=");
            if (HlsOptions.encryption_mode == ENCRYPTION_MODE_AES_128) {
                playlist->WriteString("AES-128");
            } else if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES) {
                playlist->WriteString("SAMPLE-AES");
            }
            playlist->WriteString(",URI=\"");
            playlist->WriteString(HlsOptions.encryption_key_uri);
            playlist->WriteString("\"");
            if (HlsOptions.encryption_iv_mode == ENCRYPTION_IV_MODE_RANDOM) {
                playlist->WriteString(",IV=0x");
                char iv_hex[33];
                iv_hex[32] = 0;
                AP4_FormatHex(HlsOptions.encryption_iv, 16, iv_hex);
                playlist->WriteString(iv_hex);
            }
            if (HlsOptions.encryption_key_format) {
                playlist->WriteString(",KEYFORMAT=\"");
                playlist->WriteString(HlsOptions.encryption_key_format);
                playlist->WriteString("\"");
            }
            if (HlsOptions.encryption_key_format_versions) {
                playlist->WriteString(",KEYFORMATVERSIONS=\"");
                playlist->WriteString(HlsOptions.encryption_key_format_versions);
                playlist->WriteString("\"");
            }
            playlist->WriteString("\r\n");
        }

        for (unsigned int i = 0; i < iframe_positions.ItemCount(); i++) {
            sprintf(string_buffer, "#EXTINF:%f,\r\n", iframe_durations[i]);
            playlist->WriteString(string_buffer);
            sprintf(string_buffer, "#EXT-X-BYTERANGE:%d@%lld\r\n", iframe_sizes[i],
                    iframe_positions[i]);
            playlist->WriteString(string_buffer);
            sprintf(string_buffer, HlsOptions.segment_url_template, iframe_segment_indexes[i]);
            playlist->WriteString(string_buffer);
            playlist->WriteString("\r\n");
        }

        playlist->WriteString("#EXT-X-ENDLIST\r\n");
        playlist->Release();
    }

    // update stats
    Stats.segment_count = segment_sizes.ItemCount();
    for (unsigned int i = 0; i < segment_sizes.ItemCount(); i++) {
        Stats.segments_total_size += segment_sizes[i];
        Stats.segments_total_duration += segment_durations[i];
    }
    Stats.iframe_count = iframe_sizes.ItemCount();
    for (unsigned int i = 0; i < iframe_sizes.ItemCount(); i++) {
        Stats.iframes_total_size += iframe_sizes[i];
    }
    for (unsigned int i = 0; i < iframe_positions.ItemCount(); i++) {
        if (iframe_durations[i] != 0.0) {
            double iframe_bitrate = 8.0 * (double) iframe_sizes[i] / iframe_durations[i];
            if (iframe_bitrate > Stats.max_iframe_bitrate) {
                Stats.max_iframe_bitrate = iframe_bitrate;
            }
        }
    }

    if (HlsOptions.verbose) {
        sprintf(msg, "Conversion complete, total duration=%.2f secs\n", total_duration);
        __android_log_print(ANDROID_LOG_DEBUG, "TAG", "bbbb:%s", msg);
    }

    if (segment_output) segment_output->Release();
    delete sample_encrypter;

    if (AP4_FAILED(result)) {
        __android_log_print(ANDROID_LOG_DEBUG, "eee", "dddddddddddd----------------------------");
    }

    return result;
}

/*----------------------------------------------------------------------
|   main
+---------------------------------------------------------------------*/
std::string Mp42Hls::mp42hls(int argc, char **argv) {

    auto t = std::chrono::steady_clock::now();

    char msg[256];
    if (argc < 2) {
        PrintUsageAndExit();
    }

    // default options
    HlsOptions.input = NULL;
    HlsOptions.verbose = false;
    HlsOptions.hls_version = 0;
    HlsOptions.pmt_pid = 0x100;
    HlsOptions.audio_pid = 0x101;
    HlsOptions.video_pid = 0x102;
    HlsOptions.audio_track_id = -1;
    HlsOptions.video_track_id = -1;
    HlsOptions.audio_format = AUDIO_FORMAT_TS;
    HlsOptions.output_single_file = false;
    HlsOptions.show_info = false;
    HlsOptions.index_filename = "stream.m3u8";
    HlsOptions.iframe_index_filename = NULL;
    HlsOptions.segment_filename_template = NULL;
    HlsOptions.segment_url_template = NULL;
    HlsOptions.segment_duration = 6;
    HlsOptions.segment_duration_threshold = DefaultSegmentDurationThreshold;
    HlsOptions.allow_cache = NULL;
    HlsOptions.encryption_key_hex = NULL;
    HlsOptions.encryption_mode = ENCRYPTION_MODE_NONE;
    HlsOptions.encryption_iv_mode = ENCRYPTION_IV_MODE_NONE;
    HlsOptions.encryption_key_uri = "key.bin";
    HlsOptions.encryption_key_format = NULL;
    HlsOptions.encryption_key_format_versions = NULL;
    HlsOptions.pcr_offset = AP4_MPEG2_TS_DEFAULT_PCR_OFFSET;
    AP4_SetMemory(HlsOptions.encryption_key, 0, sizeof(HlsOptions.encryption_key));
    AP4_SetMemory(HlsOptions.encryption_iv, 0, sizeof(HlsOptions.encryption_iv));
    AP4_SetMemory(&Stats, 0, sizeof(Stats));

    // parse command line
    AP4_Result result;
    char **args = argv + 1;
    while (const char *arg = *args++) {
        if (!strcmp(arg, "--verbose")) {
            HlsOptions.verbose = true;
        } else if (!strcmp(arg, "--hls-version")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --hls-version requires a number\n");
                return msg;
            }
            HlsOptions.hls_version = (unsigned int) strtoul(*args++, NULL, 10);
            if (HlsOptions.hls_version == 0) {
                sprintf(msg, "ERROR: --hls-version requires number > 0\n");
                return msg;
            }
        } else if (!strcmp(arg, "--segment-duration")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --segment-duration requires a number\n");
                return msg;
            }
            HlsOptions.segment_duration = (unsigned int) strtoul(*args++, NULL, 10);
        } else if (!strcmp(arg, "--segment-duration-threshold")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --segment-duration-threshold requires a number\n");
                return msg;
            }
            HlsOptions.segment_duration_threshold = (unsigned int) strtoul(*args++, NULL, 10);
        } else if (!strcmp(arg, "--segment-filename-template")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --segment-filename-template requires an argument\n");
                return msg;
            }
            HlsOptions.segment_filename_template = *args++;
        } else if (!strcmp(arg, "--segment-url-template")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --segment-url-template requires an argument\n");
                return msg;
            }
            HlsOptions.segment_url_template = *args++;
        } else if (!strcmp(arg, "--allow-cache")) {
            if (*args == NULL || (strcmp(*args, "NO") && strcmp(*args, "YES"))) {
                sprintf(msg, "ERROR: --allow-cache requires a YES or NO argument\n");
                return msg;
            }
            HlsOptions.allow_cache = *args++;
        } else if (!strcmp(arg, "--pmt-pid")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --pmt-pid requires a number\n");
                return msg;
            }
            HlsOptions.pmt_pid = (unsigned int) strtoul(*args++, NULL, 10);
        } else if (!strcmp(arg, "--audio-pid")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --audio-pid requires a number\n");
                return msg;
            }
            HlsOptions.audio_pid = (unsigned int) strtoul(*args++, NULL, 10);
        } else if (!strcmp(arg, "--video-pid")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --video-pid requires a number\n");
                return msg;
            }
            HlsOptions.video_pid = (unsigned int) strtoul(*args++, NULL, 10);
        } else if (!strcmp(arg, "--audio-track-id")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --audio-track-id requires a number\n");
                return msg;
            }
            HlsOptions.audio_track_id = (unsigned int) strtoul(*args++, NULL, 10);
        } else if (!strcmp(arg, "--audio-format")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --audio-format requires an argument\n");
                return msg;
            }
            const char *format = *args++;
            if (!strcmp(format, "ts")) {
                HlsOptions.audio_format = AUDIO_FORMAT_TS;
            } else if (!strcmp(format, "packed")) {
                HlsOptions.audio_format = AUDIO_FORMAT_PACKED;
            } else {
                sprintf(msg, "ERROR: unknown audio format\n");
                __android_log_print(ANDROID_LOG_DEBUG, "TAG", "ERROR: unknown audio format");
                return msg;
            }
        } else if (!strcmp(arg, "--video-track-id")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --video-track-id requires a number\n");
                return msg;
            }
            HlsOptions.video_track_id = (unsigned int) strtoul(*args++, NULL, 10);
        } else if (!strcmp(arg, "--pcr-offset")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --pcr-offset requires a number\n");
                return msg;
            }
            HlsOptions.pcr_offset = (unsigned int) strtoul(*args++, NULL, 10);
        } else if (!strcmp(arg, "--output-single-file")) {
            HlsOptions.output_single_file = true;
        } else if (!strcmp(arg, "--index-filename")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --index-filename requires a filename\n");
                return msg;
            }
            HlsOptions.index_filename = *args++;
        } else if (!strcmp(arg, "--iframe-index-filename")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --iframe-index-filename requires a filename\n");
                return msg;
            }
            HlsOptions.iframe_index_filename = *args++;
        } else if (!strcmp(arg, "--show-info")) {
            HlsOptions.show_info = true;
        } else if (!strcmp(arg, "--encryption-key")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --encryption-key requires an argument\n");
                return msg;
            }
            HlsOptions.encryption_key_hex = *args++;
            result = AP4_ParseHex(HlsOptions.encryption_key_hex, HlsOptions.encryption_key, 16);
            if (AP4_FAILED(result)) {
                sprintf(msg, "ERROR: invalid hex key\n");
                return msg;
            }
            if (HlsOptions.encryption_mode == ENCRYPTION_MODE_NONE) {
                HlsOptions.encryption_mode = ENCRYPTION_MODE_AES_128;
            }
        } else if (!strcmp(arg, "--encryption-mode")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --encryption-mode requires an argument\n");
                return msg;
            }
            if (strncmp(*args, "AES-128", 7) == 0) {
                HlsOptions.encryption_mode = ENCRYPTION_MODE_AES_128;
            } else if (strncmp(*args, "SAMPLE-AES", 10) == 0) {
                HlsOptions.encryption_mode = ENCRYPTION_MODE_SAMPLE_AES;
            } else {
                sprintf(msg, "ERROR: unknown encryption mode\n");
                return msg;
            }
            ++args;
        } else if (!strcmp(arg, "--encryption-iv-mode")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --encryption-iv-mode requires an argument\n");
                return msg;
            }
            if (strncmp(*args, "sequence", 8) == 0) {
                HlsOptions.encryption_iv_mode = ENCRYPTION_IV_MODE_SEQUENCE;
            } else if (strncmp(*args, "random", 6) == 0) {
                HlsOptions.encryption_iv_mode = ENCRYPTION_IV_MODE_RANDOM;
            } else if (strncmp(*args, "fps", 3) == 0) {
                HlsOptions.encryption_iv_mode = ENCRYPTION_IV_MODE_FPS;
            } else {
                sprintf(msg, "ERROR: unknown encryption IV mode\n");
                return msg;
            }
            ++args;
        } else if (!strcmp(arg, "--encryption-key-uri")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --encryption-key-uri requires an argument\n");
                return msg;
            }
            HlsOptions.encryption_key_uri = *args++;
        } else if (!strcmp(arg, "--encryption-key-format")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --encryption-key-format requires an argument\n");
                return msg;
            }
            HlsOptions.encryption_key_format = *args++;
        } else if (!strcmp(arg, "--encryption-key-format-versions")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --encryption-key-format-versions requires an argument\n");
                return msg;
            }
            HlsOptions.encryption_key_format_versions = *args++;
        } else if (!strcmp(arg, "--encryption-key-line")) {
            if (*args == NULL) {
                sprintf(msg, "ERROR: --encryption-key-line requires an argument\n");
                return msg;
            }
            HlsOptions.encryption_key_lines.Append(*args++);
        } else if (HlsOptions.input == NULL) {
            HlsOptions.input = arg;
        } else {
            sprintf(msg, "ERROR: unexpected argument: %s\n", arg);
            break;
        }
    }

    // check args
    if (HlsOptions.input == NULL) {
        sprintf(msg, "ERROR: missing input file name\n");
        return msg;
    }
    if (HlsOptions.encryption_mode == ENCRYPTION_MODE_NONE &&
        HlsOptions.encryption_key_lines.ItemCount() != 0) {
        sprintf(msg,
                "ERROR: --encryption-key-line requires --encryption-key and --encryption-key-mode\n");
        return msg;
    }
    if (HlsOptions.encryption_mode != ENCRYPTION_MODE_NONE &&
        HlsOptions.encryption_key_hex == NULL) {
        sprintf(msg, "ERROR: no encryption key specified\n");
        return msg;
    }
    if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES && HlsOptions.hls_version > 0 &&
        HlsOptions.hls_version < 5) {
        HlsOptions.hls_version = 5;
        sprintf(msg,
                "WARNING: forcing version to 5 in order to support SAMPLE-AES encryption\n");
        __android_log_print(ANDROID_LOG_DEBUG, "TAG", "Hls version=%s", msg);
    }
    if (HlsOptions.iframe_index_filename && HlsOptions.hls_version > 0 &&
        HlsOptions.hls_version < 4) {
        sprintf(msg,
                "WARNING: forcing version to 4 in order to support I-FRAME-ONLY playlists\n");
        HlsOptions.hls_version = 4;
        __android_log_print(ANDROID_LOG_DEBUG, "TAG", "Hls version=%s", msg);
    }
    if (HlsOptions.encryption_iv_mode == ENCRYPTION_IV_MODE_NONE &&
        HlsOptions.encryption_mode != ENCRYPTION_MODE_NONE) {
        if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES) {
            // sequence-mode IVs don't work well with i-frame only playlists, use random instead
            HlsOptions.encryption_iv_mode = ENCRYPTION_IV_MODE_RANDOM;
        } else {
            HlsOptions.encryption_iv_mode = ENCRYPTION_IV_MODE_SEQUENCE;
        }
    }
    if ((HlsOptions.encryption_key_format || HlsOptions.encryption_key_format_versions) &&
        HlsOptions.hls_version > 0 && HlsOptions.hls_version < 5) {
        HlsOptions.hls_version = 5;
        sprintf(msg,
                "WARNING: forcing version to 5 in order to support KEYFORMAT and/or KEYFORMATVERSIONS\n");
        __android_log_print(ANDROID_LOG_DEBUG, "TAG", "Hls version=%s", msg);
    }
    if (HlsOptions.output_single_file && HlsOptions.hls_version > 0 && HlsOptions.hls_version < 4) {
        HlsOptions.hls_version = 4;
        sprintf(msg, "WARNING: forcing version to 4 in order to support single file output\n");
        __android_log_print(ANDROID_LOG_DEBUG, "TAG", "Hls version=%s", msg);
    }
    if (HlsOptions.hls_version == 0) {
        // default version is 3 for cleartext or AES-128 encryption, and 5 for SAMPLE-AES
        if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES) {
            HlsOptions.hls_version = 5;
        } else if (HlsOptions.output_single_file || HlsOptions.iframe_index_filename) {
            HlsOptions.hls_version = 4;
        } else {
            HlsOptions.hls_version = 3;
        }
    }

    if (HlsOptions.verbose && HlsOptions.show_info) {
        sprintf(msg, "WARNING: --verbose will be ignored because --show-info is selected\n");
        __android_log_print(ANDROID_LOG_DEBUG, "TAG", "Hls version=%s", msg);
        HlsOptions.verbose = false;
    }

    // compute some derived values
    if (HlsOptions.iframe_index_filename == NULL) {
        if (HlsOptions.hls_version >= 4) {
            HlsOptions.iframe_index_filename = "iframes.m3u8";
        }
    }
    if (HlsOptions.audio_format == AUDIO_FORMAT_TS) {
        if (HlsOptions.segment_filename_template == NULL) {
            if (HlsOptions.output_single_file) {
                HlsOptions.segment_filename_template = "stream.ts";
            } else {
                HlsOptions.segment_filename_template = "segment-%d.ts";
            }
        }
        if (HlsOptions.segment_url_template == NULL) {
            if (HlsOptions.output_single_file) {
                HlsOptions.segment_url_template = "stream.ts";
            } else {
                HlsOptions.segment_url_template = "segment-%d.ts";
            }
        }
    }

    if (HlsOptions.encryption_iv_mode == ENCRYPTION_IV_MODE_FPS) {
        if (HlsOptions.encryption_key_hex == NULL ||
            AP4_StringLength(HlsOptions.encryption_key_hex) != 64) {
            sprintf(msg,
                    "ERROR: 'fps' IV mode requires a 32 byte key value (64 characters in hex)\n");
            return msg;
        }
        result = AP4_ParseHex(HlsOptions.encryption_key_hex + 32, HlsOptions.encryption_iv, 16);
        if (AP4_FAILED(result)) {
            sprintf(msg, "ERROR: invalid hex IV\n");
            return msg;
        }
    } else if (HlsOptions.encryption_iv_mode == ENCRYPTION_IV_MODE_RANDOM) {
        result = AP4_System_GenerateRandomBytes(HlsOptions.encryption_iv,
                                                sizeof(HlsOptions.encryption_iv));
        if (AP4_FAILED(result)) {
            sprintf(msg, "ERROR: failed to get random IV (%d)\n", result);
            return msg;
        }
    }

    // create the input stream
    AP4_ByteStream *input = NULL;
    result = AP4_FileByteStream::Create(HlsOptions.input, AP4_FileByteStream::STREAM_MODE_READ,
                                        input);
    if (AP4_FAILED(result)) {
        sprintf(msg, "ERROR: cannot open input (%d)\n", result);
        return msg;
    }

    // open the file
    AP4_File *input_file = new AP4_File(*input, true);

    // get the movie
    AP4_SampleDescription *sample_description;
    AP4_Movie *movie = input_file->GetMovie();
    if (movie == NULL) {
        sprintf(msg, "ERROR: no movie in file\n");
        return msg;
    }

    // get the audio and video tracks
    AP4_Track *audio_track = NULL;
    if (HlsOptions.audio_track_id == -1) {
        audio_track = movie->GetTrack(AP4_Track::TYPE_AUDIO);
    } else if (HlsOptions.audio_track_id > 0) {
        audio_track = movie->GetTrack((AP4_UI32) HlsOptions.audio_track_id);
        if (audio_track == NULL) {
            sprintf(msg, "ERROR: audio track ID %d not found\n", HlsOptions.audio_track_id);
            return msg;
        }
        if (audio_track->GetType() != AP4_Track::TYPE_AUDIO) {
            sprintf(msg, "ERROR: track ID %d is not an audio track\n", HlsOptions.audio_track_id);
            return msg;
        }
    }

    AP4_Track *video_track = NULL;
    if (HlsOptions.video_track_id == -1) {
        video_track = movie->GetTrack(AP4_Track::TYPE_VIDEO);
    } else if (HlsOptions.video_track_id > 0) {
        video_track = movie->GetTrack((AP4_UI32) HlsOptions.video_track_id);
        if (video_track == NULL) {
            sprintf(msg, "ERROR: video track ID %d not found\n", HlsOptions.video_track_id);
            return msg;
        }
        if (video_track->GetType() != AP4_Track::TYPE_VIDEO) {
            sprintf(msg, "ERROR: track ID %d is not a video track\n", HlsOptions.video_track_id);
            return msg;
        }
    }

    if (audio_track == NULL && video_track == NULL) {
        sprintf(msg, "ERROR: no suitable tracks found\n");
        __android_log_print(ANDROID_LOG_DEBUG, "TAG", "Hls version=%s", msg);
        delete input_file;
        input->Release();
        return msg;
    }

    if (HlsOptions.audio_format == AUDIO_FORMAT_PACKED && video_track != NULL) {
        if (audio_track == NULL) {
            sprintf(msg, "ERROR: packed audio format requires an audio track\n");
            return msg;
        }
        sprintf(msg, "WARNING: ignoring video track because of the packed audio format\n");
        __android_log_print(ANDROID_LOG_DEBUG, "TAG",
                            "WARNING: ignoring video track because of the packed audio format");
        video_track = NULL;
    }
    if (video_track == NULL) {
        HlsOptions.segment_duration_threshold = 0;
    }

    // create the appropriate readers
    AP4_LinearReader *linear_reader = NULL;
    SampleReader *audio_reader = NULL;
    SampleReader *video_reader = NULL;
    if (movie->HasFragments()) {
        // create a linear reader to get the samples
        linear_reader = new AP4_LinearReader(*movie, input);

        if (audio_track) {
            linear_reader->EnableTrack(audio_track->GetId());
            audio_reader = new FragmentedSampleHlsReader(*linear_reader, audio_track->GetId());
        }
        if (video_track) {
            linear_reader->EnableTrack(video_track->GetId());
            video_reader = new FragmentedSampleHlsReader(*linear_reader, video_track->GetId());
        }
    } else {
        if (audio_track) {
            audio_reader = new TrackSampleHlsReader(*audio_track);
        }
        if (video_track) {
            video_reader = new TrackSampleHlsReader(*video_track);
        }
    }

    AP4_Mpeg2TsWriter *ts_writer = NULL;
    AP4_Mpeg2TsWriter::SampleStream *audio_stream = NULL;
    AP4_Mpeg2TsWriter::SampleStream *video_stream = NULL;
    AP4_UI08 nalu_length_size = 0;
    PackedAudioWriter *packed_writer = NULL;
    if (HlsOptions.audio_format == AUDIO_FORMAT_PACKED) {
        packed_writer = new PackedAudioWriter();

        // figure out the file extensions if needed
        sample_description = audio_track->GetSampleDescription(0);
        if (sample_description == NULL) {
            sprintf(msg, "ERROR: unable to parse audio sample description\n");
            __android_log_print(ANDROID_LOG_DEBUG, "TAG", "Hls version=%s", msg);
            goto end;
        }
        if (HlsOptions.segment_filename_template == NULL ||
            HlsOptions.segment_url_template == NULL) {
            const char *default_stream_name = "stream.es";
            const char *default_stream_pattern = "segment-%d.es";
            if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_MP4A) {
                AP4_MpegAudioSampleDescription *mpeg_audio_desc = AP4_DYNAMIC_CAST(
                        AP4_MpegAudioSampleDescription, sample_description);
                if (mpeg_audio_desc == NULL ||
                    !(mpeg_audio_desc->GetObjectTypeId() == AP4_OTI_MPEG4_AUDIO ||
                      mpeg_audio_desc->GetObjectTypeId() == AP4_OTI_MPEG2_AAC_AUDIO_LC ||
                      mpeg_audio_desc->GetObjectTypeId() == AP4_OTI_MPEG2_AAC_AUDIO_MAIN)) {
                    sprintf(msg, "ERROR: only AAC audio is supported\n");
                    __android_log_print(ANDROID_LOG_DEBUG, "TAG", "Hls version=%s", msg);
                    return msg;
                }
                default_stream_name = "stream.aac";
                default_stream_pattern = "segment-%d.aac";
            } else if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_AC_3) {
                default_stream_name = "stream.ac3";
                default_stream_pattern = "segment-%d.ac3";
            } else if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_EC_3) {
                default_stream_name = "stream.ec3";
                default_stream_pattern = "segment-%d.ec3";
            } else if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_AC_4) {
                default_stream_name = "stream.ac4";
                default_stream_pattern = "segment-%d.ac4";
            }

            // override the segment names
            if (HlsOptions.segment_filename_template == NULL) {
                if (HlsOptions.output_single_file) {
                    HlsOptions.segment_filename_template = default_stream_name;
                } else {
                    HlsOptions.segment_filename_template = default_stream_pattern;
                }
            }
            if (HlsOptions.segment_url_template == NULL) {
                if (HlsOptions.output_single_file) {
                    HlsOptions.segment_url_template = default_stream_name;
                } else {
                    HlsOptions.segment_url_template = default_stream_pattern;
                }
            }
        }
    } else {
        // create an MPEG2 TS Writer
        ts_writer = new AP4_Mpeg2TsWriter(HlsOptions.pmt_pid);

        // add the audio stream
        if (audio_track) {
            sample_description = audio_track->GetSampleDescription(0);
            if (sample_description == NULL) {
                sprintf(msg, "ERROR: unable to parse audio sample description\n");
                __android_log_print(ANDROID_LOG_DEBUG, "TAG", "Hls version=%s", msg);
                goto end;
            }

            unsigned int stream_type = 0;
            unsigned int stream_id = 0;
            if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_MP4A) {
                if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES) {
                    stream_type = AP4_MPEG2_STREAM_TYPE_SAMPLE_AES_ISO_IEC_13818_7;
                } else {
                    stream_type = AP4_MPEG2_STREAM_TYPE_ISO_IEC_13818_7;
                }
                stream_id = AP4_MPEG2_TS_DEFAULT_STREAM_ID_AUDIO;
            } else if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_AC_3) {
                if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES) {
                    stream_type = AP4_MPEG2_STREAM_TYPE_SAMPLE_AES_ATSC_AC3;
                } else {
                    stream_type = AP4_MPEG2_STREAM_TYPE_ATSC_AC3;
                }
                stream_id = AP4_MPEG2_TS_STREAM_ID_PRIVATE_STREAM_1;
            } else if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_EC_3) {
                if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES) {
                    stream_type = AP4_MPEG2_STREAM_TYPE_SAMPLE_AES_ATSC_EAC3;
                } else {
                    stream_type = AP4_MPEG2_STREAM_TYPE_ATSC_EAC3;
                }
                stream_id = AP4_MPEG2_TS_STREAM_ID_PRIVATE_STREAM_1;
            } else {
                sprintf(msg, "ERROR: audio codec not supported\n");
                __android_log_print(ANDROID_LOG_DEBUG, "TAG", "Hls version=%s", msg);
                return msg;
            }
            if (stream_type == AP4_MPEG2_STREAM_TYPE_ATSC_EAC3) {
                // E-AC-3 descriptor
                unsigned int number_of_channels = 0;
                AP4_String track_language;
                AP4_Dec3Atom *dec3 = AP4_DYNAMIC_CAST(AP4_Dec3Atom,
                                                      sample_description->GetDetails().GetChild(
                                                              AP4_ATOM_TYPE_DEC3));
                AP4_BitWriter bits(8);
                bits.Write(0xCC, 8);
                bits.Write(0x06, 8);    // fixed value
                bits.Write(0xC0,
                           8);    // reserved, bsid_flag, mainid_flag, asvc_flag, mixinfoexists, substream1_flag, substream2_flag and substream3_flag
                bits.Write(24, 5);      // reserved, full_service_flag and service_type
                if (dec3->GetSubStreams()[0].acmod == 0) {
                    number_of_channels = 1;
                } else if (dec3->GetSubStreams()[0].acmod == 1) {
                    number_of_channels = 0;
                } else if (dec3->GetSubStreams()[0].acmod == 2) {
                    number_of_channels = 2;
                } else {
                    number_of_channels = 4;
                }
                if (dec3->GetSubStreams()[0].num_dep_sub > 0) {
                    number_of_channels = 5;
                }
                bits.Write(number_of_channels, 3);              // number_of_channels
                bits.Write(4,
                           3);                               // language_flag, language_flag_2, reserved
                bits.Write(dec3->GetSubStreams()[0].bsid, 5);   // bsid
                track_language = audio_track->GetTrackLanguage();
                if (track_language.GetLength() == 3) {
                    bits.Write(track_language.GetChars()[0], 8);
                    bits.Write(track_language.GetChars()[1], 8);
                    bits.Write(track_language.GetChars()[2], 8);
                } else {
                    bits.Write(0x75, 8);
                    bits.Write(0x6E, 8);
                    bits.Write(0x64, 8);
                }
                // setup the audio stream
                result = ts_writer->SetAudioStream(audio_track->GetMediaTimeScale(),
                                                   stream_type,
                                                   stream_id,
                                                   audio_stream,
                                                   HlsOptions.audio_pid,
                                                   bits.GetData(), 8,
                                                   HlsOptions.pcr_offset);
            } else {
                // setup the audio stream
                result = ts_writer->SetAudioStream(audio_track->GetMediaTimeScale(),
                                                   stream_type,
                                                   stream_id,
                                                   audio_stream,
                                                   HlsOptions.audio_pid,
                                                   NULL, 0,
                                                   HlsOptions.pcr_offset);
            }
            if (AP4_FAILED(result)) {
                sprintf(msg, "could not create audio stream (%d)\n", result);
                goto end;
            }
        }

        // add the video stream
        if (video_track) {
            sample_description = video_track->GetSampleDescription(0);
            if (sample_description == NULL) {
                sprintf(msg, "ERROR: unable to parse video sample description\n");
                __android_log_print(ANDROID_LOG_DEBUG, "TAG", "Hls version=%s", msg);
                goto end;
            }

            // decide on the stream type
            unsigned int stream_type = 0;
            unsigned int stream_id = AP4_MPEG2_TS_DEFAULT_STREAM_ID_VIDEO;
            if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_AVC1 ||
                sample_description->GetFormat() == AP4_SAMPLE_FORMAT_AVC2 ||
                sample_description->GetFormat() == AP4_SAMPLE_FORMAT_AVC3 ||
                sample_description->GetFormat() == AP4_SAMPLE_FORMAT_AVC4 ||
                sample_description->GetFormat() == AP4_SAMPLE_FORMAT_DVAV ||
                sample_description->GetFormat() == AP4_SAMPLE_FORMAT_DVA1) {
                if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES) {
                    stream_type = AP4_MPEG2_STREAM_TYPE_SAMPLE_AES_AVC;
                    AP4_AvcSampleDescription *avc_desc = AP4_DYNAMIC_CAST(AP4_AvcSampleDescription,
                                                                          sample_description);
                    if (avc_desc == NULL) {
                        sprintf(msg, "ERROR: not a proper AVC track\n");
                        return msg;
                    }
                    nalu_length_size = avc_desc->GetNaluLengthSize();
                } else {
                    stream_type = AP4_MPEG2_STREAM_TYPE_AVC;
                }
            } else if (sample_description->GetFormat() == AP4_SAMPLE_FORMAT_HEV1 ||
                       sample_description->GetFormat() == AP4_SAMPLE_FORMAT_HVC1 ||
                       sample_description->GetFormat() == AP4_SAMPLE_FORMAT_DVHE ||
                       sample_description->GetFormat() == AP4_SAMPLE_FORMAT_DVH1) {
                stream_type = AP4_MPEG2_STREAM_TYPE_HEVC;
            } else {
                sprintf(msg, "ERROR: video codec not supported\n");
                __android_log_print(ANDROID_LOG_DEBUG, "TAG", "ERROR: video codec not supported");
                return msg;
            }
            if (HlsOptions.encryption_mode == ENCRYPTION_MODE_SAMPLE_AES) {
                if (stream_type != AP4_MPEG2_STREAM_TYPE_SAMPLE_AES_AVC) {
                    sprintf(msg,
                            "ERROR: AES-SAMPLE encryption can only be used with H.264 video\n");
                    return msg;
                }
            }

            // setup the video stream
            result = ts_writer->SetVideoStream(video_track->GetMediaTimeScale(),
                                               stream_type,
                                               stream_id,
                                               video_stream,
                                               HlsOptions.video_pid,
                                               NULL, 0,
                                               HlsOptions.pcr_offset);
            if (AP4_FAILED(result)) {
                sprintf(msg, "could not create video stream (%d)\n", result);
                goto end;
            }
        }
    }

    result = WriteSamples(ts_writer, packed_writer,
                          audio_track, audio_reader, audio_stream,
                          video_track, video_reader, video_stream,
                          HlsOptions.segment_duration_threshold,
                          nalu_length_size);

    if (AP4_FAILED(result)) {
        sprintf(msg, "ERROR: failed to write samples2 (%d)\n", result);
        __android_log_print(ANDROID_LOG_DEBUG, "TAG", "AP4_FAILED:%s", msg);
        return msg;
    }

    if (HlsOptions.show_info) {
        double average_segment_bitrate = 0.0;
        if (Stats.segments_total_duration != 0.0) {
            average_segment_bitrate =
                    8.0 * (double) Stats.segments_total_size / Stats.segments_total_duration;
        }
        double average_iframe_bitrate = 0.0;
        if (Stats.segments_total_duration != 0.0) {
            average_iframe_bitrate =
                    8.0 * (double) Stats.iframes_total_size / Stats.segments_total_duration;
        }

        double frame_rate = 0.0;
        if (video_track && (Stats.segments_total_duration != 0.0)) {
            double sample_count = (double) video_track->GetSampleCount();
            double media_duration = (double) video_track->GetMediaDuration();
            double timescale = (double) video_track->GetMediaTimeScale();
            if (media_duration > 0.0) {
                frame_rate = sample_count / (media_duration / timescale);
            }
        }

        printf(
                "{\n"
        );
        printf(
                "  \"stats\": {\n"
                "    \"duration\": %f,\n"
                "    \"avg_segment_bitrate\": %f,\n"
                "    \"max_segment_bitrate\": %f,\n"
                "    \"avg_iframe_bitrate\": %f,\n"
                "    \"max_iframe_bitrate\": %f,\n"
                "    \"frame_rate\": %f\n"
                "  }",
                (double) movie->GetDurationMs() / 1000.0,
                average_segment_bitrate,
                Stats.max_segment_bitrate,
                average_iframe_bitrate,
                Stats.max_iframe_bitrate,
                frame_rate
        );
        if (audio_track) {
            AP4_String codec;
            AP4_SampleDescription *sdesc = audio_track->GetSampleDescription(0);
            if (sdesc) {
                sdesc->GetCodecString(codec);
            }
            printf(
                    ",\n"
                    "  \"audio\": {\n"
                    "    \"codec\": \"%s\"\n"
                    "  }",
                    codec.GetChars()
            );
        }
        if (video_track) {
            AP4_String codec;
            AP4_UI16 width = (AP4_UI16) (video_track->GetWidth() / 65536.0);
            AP4_UI16 height = (AP4_UI16) (video_track->GetHeight() / 65536.0);
            AP4_SampleDescription *sdesc = video_track->GetSampleDescription(0);
            if (sdesc) {
                sdesc->GetCodecString(codec);
                AP4_VideoSampleDescription *video_desc = AP4_DYNAMIC_CAST(
                        AP4_VideoSampleDescription, sdesc);
                if (video_desc) {
                    width = video_desc->GetWidth();
                    height = video_desc->GetHeight();
                }
            }
            printf(
                    ",\n"
                    "  \"video\": {\n"
                    "    \"codec\": \"%s\",\n"
                    "    \"width\": %d,\n"
                    "    \"height\": %d\n"
                    "  }",
                    codec.GetChars(),
                    width,
                    height
            );
        }
        printf(
                "\n"
                "}\n"
        );
    }

    end:
    delete ts_writer;
    delete packed_writer;
    delete input_file;
    input->Release();
    delete linear_reader;
    delete audio_reader;
    delete video_reader;


    auto hs = std::chrono::steady_clock::now();
    auto cs = std::chrono::duration_cast<std::chrono::milliseconds>(hs - t).count();
    __android_log_print(ANDROID_LOG_DEBUG, "TAG", "视频转换耗时:%lld ms", cs);

    return result == AP4_SUCCESS ? "success" : "failure";
}


