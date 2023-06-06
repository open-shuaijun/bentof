//
// Created by SJ on 2023/6/4.
//

#ifndef BENTO_MP4MAKER_H

#include <ctype.h>
#include "../Core/Ap4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <string>
#include "android/log.h"
#include "chrono"

#define BENTO_MP4MAKER_H

using namespace std;


/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
#define BANNER "MP4 Elementary Stream Multiplexer - Version 3.0\n"\
               "(Bento4 Version " AP4_VERSION_STRING ")\n"\
               "(c) 2002-20020 Axiomatic Systems, LLC"

const unsigned int AP4_MUX_DEFAULT_VIDEO_FRAME_RATE = 24;
const unsigned int AP4_MUX_READ_BUFFER_SIZE = 65536;

/*----------------------------------------------------------------------
|   globals
+---------------------------------------------------------------------*/
static struct {
    bool verbose;
} Options;

/*----------------------------------------------------------------------
|   SampleOrder
+---------------------------------------------------------------------*/
struct SampleOrder {
    SampleOrder(AP4_UI32 decode_order, AP4_UI32 display_order) :
            m_DecodeOrder(decode_order),
            m_DisplayOrder(display_order) {}

    AP4_UI32 m_DecodeOrder;
    AP4_UI32 m_DisplayOrder;
};

/*----------------------------------------------------------------------
|   Parameter
+---------------------------------------------------------------------*/
struct Parameter {
    Parameter(const char *name, const char *value) :
            m_Name(name),
            m_Value(value) {}

    AP4_String m_Name;
    AP4_String m_Value;
};


/*----------------------------------------------------------------------
|   SampleFileStorage
+---------------------------------------------------------------------*/
class SampleFileStorage {
public:
    static AP4_Result Create(const char *basename, SampleFileStorage *&sample_file_storage);

    ~SampleFileStorage() {
        m_Stream->Release();
        remove(m_Filename.GetChars());
    }

    AP4_ByteStream *GetStream() { return m_Stream; }

private:
    SampleFileStorage(const char *basename) : m_Stream(NULL) {
        AP4_Size name_length = (AP4_Size) AP4_StringLength(basename);
        char *filename = new char[name_length + 2];
        AP4_CopyMemory(filename, basename, name_length);
        filename[name_length] = '_';
        filename[name_length + 1] = '\0';
        m_Filename = filename;
        delete[] filename;
    }

    AP4_ByteStream *m_Stream;
    AP4_String m_Filename;
};


class Mp4Mux {
public:
    static Mp4Mux getInstance() {
        static Mp4Mux ins;
        return ins;
    };

//    string bboxh264(const string &path);
    string bboxh264(char** agrv);

    void PrintUsageAndExit();

    void SortSamples(SampleOrder *array, unsigned int n);

    AP4_Result ParseParameters(const char *params_str, AP4_Array<Parameter> &parameters);

    void AddAc3Track(AP4_Movie &movie, const char *input_name, AP4_Array<Parameter> &parameters,
                     SampleFileStorage &sample_storage);

    string main_func(int argc, char **argv);
//    int main_func(char *input_path, char *out_path);

    void
    AddMp4Tracks(AP4_Movie &movie, const char *input_name, AP4_Array<Parameter> &parameters,
                 AP4_Array<AP4_UI32> &  /*brands*/);

    void
    AddH265DoviTrack(AP4_Movie &movie, const char *input_name, AP4_Array<Parameter> &parameters,
                     AP4_Array<AP4_UI32> &brands, SampleFileStorage &sample_storage);

    void AddH265Track(AP4_Movie &movie, const char *input_name, AP4_Array<Parameter> &parameters,
                      AP4_Array<AP4_UI32> &brands, SampleFileStorage &sample_storage);

    void
    AddH264DoviTrack(AP4_Movie &movie, const char *input_name, AP4_Array<Parameter> &parameters,
                     AP4_Array<AP4_UI32> &brands, SampleFileStorage &sample_storage);

    void
    AddH264Track(AP4_Movie &movie,
                 const char *input_name,
                 AP4_Array<Parameter> &parameters,
                 AP4_Array<AP4_UI32> &brands,
                 SampleFileStorage &sample_storage);

    void
    AddAc4Track(AP4_Movie &movie,
                const char *input_name,
                AP4_Array<Parameter> &parameters,
                SampleFileStorage &sample_storage);

    void
    AddEac3Track(AP4_Movie &movie,
                 const char *input_name,
                 AP4_Array<Parameter> &parameters,
                 SampleFileStorage &sample_storage);

    void
    AddAacTrack(AP4_Movie &movie,
                const char *input_name,
                AP4_Array<Parameter> &parameters,
                SampleFileStorage &sample_storage);

    static const char *
    GetLanguageFromParameters(AP4_Array<Parameter> &parameters, const char *defaut_value);

    static AP4_Result CheckDoviInputParameters(AP4_Array<Parameter> &parameters);

    static AP4_Result
    ComputeDoviLevel(AP4_UI32 video_width, AP4_UI32 video_height, double frame_rate,
                     AP4_UI32 &dv_level);

private:
    Mp4Mux() {};
};


#endif //BENTO_MP4MAKER_H
