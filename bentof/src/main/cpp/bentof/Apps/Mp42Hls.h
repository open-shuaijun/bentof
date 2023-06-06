//
// Created by SJ on 2023/6/6.
//

#ifndef BENTO_MP42HLS_H
#define BENTO_MP42HLS_H

#include <stdio.h>
#include <stdlib.h>
#include "string"
#include "../Core/Ap4.h"
#include "../Crypto/Ap4StreamCipher.h"
#include "../Codecs/Ap4Mp4AudioInfo.h"
#include "android/log.h"
#include "chrono"

#endif //BENTO_MP42HLS_H




/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
#define BANNER "MP4 To HLS File Converter - Version 1.2\n"\
               "(Bento4 Version " AP4_VERSION_STRING ")\n"\
               "(c) 2002-2018 Axiomatic Systems, LLC"

/*----------------------------------------------------------------------
|   options
+---------------------------------------------------------------------*/
typedef enum {
    ENCRYPTION_MODE_NONE,
    ENCRYPTION_MODE_AES_128,
    ENCRYPTION_MODE_SAMPLE_AES
} EncryptionMode;

typedef enum {
    ENCRYPTION_IV_MODE_NONE,
    ENCRYPTION_IV_MODE_SEQUENCE,
    ENCRYPTION_IV_MODE_RANDOM,
    ENCRYPTION_IV_MODE_FPS
} EncryptionIvMode;

typedef enum {
    AUDIO_FORMAT_TS,
    AUDIO_FORMAT_PACKED
} AudioFormat;

static struct _HlsOptions {
    const char *input;
    bool verbose;
    unsigned int hls_version;
    unsigned int pmt_pid;
    unsigned int audio_pid;
    unsigned int video_pid;
    int audio_track_id;
    int video_track_id;
    AudioFormat audio_format;
    const char *index_filename;
    const char *iframe_index_filename;
    bool output_single_file;
    bool show_info;
    const char *segment_filename_template;
    const char *segment_url_template;
    unsigned int segment_duration;
    unsigned int segment_duration_threshold;
    const char *allow_cache;
    const char *encryption_key_hex;
    AP4_UI08 encryption_key[16];
    AP4_UI08 encryption_iv[16];
    EncryptionMode encryption_mode;
    EncryptionIvMode encryption_iv_mode;
    const char *encryption_key_uri;
    const char *encryption_key_format;
    const char *encryption_key_format_versions;
    AP4_Array<AP4_String> encryption_key_lines;
    AP4_UI64 pcr_offset;
} HlsOptions;

static struct _Stats {
    AP4_UI64 segments_total_size;
    double segments_total_duration;
    AP4_UI32 segment_count;
    double max_segment_bitrate;
    AP4_UI64 iframes_total_size;
    AP4_UI32 iframe_count;
    double max_iframe_bitrate;
} Stats;

/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
static const unsigned int DefaultSegmentDurationThreshold = 15; // milliseconds

const AP4_UI08 AP4_MPEG2_STREAM_TYPE_SAMPLE_AES_AVC = 0xDB;
const AP4_UI08 AP4_MPEG2_STREAM_TYPE_SAMPLE_AES_ISO_IEC_13818_7 = 0xCF;
const AP4_UI08 AP4_MPEG2_STREAM_TYPE_SAMPLE_AES_ATSC_AC3 = 0xC1;
const AP4_UI08 AP4_MPEG2_STREAM_TYPE_SAMPLE_AES_ATSC_EAC3 = 0xC2;
const AP4_UI08 AP4_MPEG2_PRIVATE_DATA_INDICATOR_DESCRIPTOR_TAG = 15;
const AP4_UI08 AP4_MPEG2_REGISTRATION_DESCRIPTOR_TAG = 5;

/*----------------------------------------------------------------------
|   SampleReader
+---------------------------------------------------------------------*/
class SampleReader {
public:
    virtual ~SampleReader() {}

    virtual AP4_Result ReadSample(AP4_Sample &sample, AP4_DataBuffer &sample_data) = 0;
};

/*----------------------------------------------------------------------
|   TrackSampleReader
+---------------------------------------------------------------------*/
class TrackSampleHlsReader : public SampleReader {
public:
    TrackSampleHlsReader(AP4_Track &track) : m_Track(track), m_SampleIndex(0) {}

    AP4_Result ReadSample(AP4_Sample &sample, AP4_DataBuffer &sample_data);

private:
    AP4_Track &m_Track;
    AP4_Ordinal m_SampleIndex;
};


/*----------------------------------------------------------------------
|   FragmentedSampleReader
+---------------------------------------------------------------------*/
class FragmentedSampleHlsReader : public SampleReader {
public:
    FragmentedSampleHlsReader(AP4_LinearReader &fragment_reader, AP4_UI32 track_id) :
            m_FragmentReader(fragment_reader), m_TrackId(track_id) {
        fragment_reader.EnableTrack(track_id);
    }

    AP4_Result ReadSample(AP4_Sample &sample, AP4_DataBuffer &sample_data);

private:
    AP4_LinearReader &m_FragmentReader;
    AP4_UI32 m_TrackId;
};


class Mp42Hls {
private:
    Mp42Hls() {};
public:
    static Mp42Hls &getInstance() {
        static Mp42Hls hls;
        return hls;
    }

    std::string mp42hls(int argc, char **argv);
};