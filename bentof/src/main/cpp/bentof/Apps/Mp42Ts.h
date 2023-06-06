//
// Created by SJ on 2023/6/6.
//

#ifndef BENTO_MP42TS_H
#define BENTO_MP42TS_H

#include <stdio.h>
#include <stdlib.h>
#include "string"
#include "Ap4.h"

#endif //BENTO_MP42TS_H


/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
#define TS_BANNER "MP4 To MPEG2-TS File Converter - Version 1.3\n"\
               "(Bento4 Version " AP4_VERSION_STRING ")\n"\
               "(c) 2002-2018 Axiomatic Systems, LLC"

/*----------------------------------------------------------------------
|   options
+---------------------------------------------------------------------*/
struct _TsOptions {
    unsigned int pmt_pid;
    unsigned int audio_pid;
    unsigned int video_pid;
    bool verbose;
    const char *playlist;
    unsigned int playlist_hls_version;
    const char *input;
    const char *output;
    unsigned int segment_duration;
    unsigned int segment_duration_threshold;
    unsigned int pcr_offset;
} TsOptions;


class Mp42Ts {

private :
    Mp42Ts() {};
public :
    static Mp42Ts &getInstance() {
        static Mp42Ts ts;
        return ts;
    };

    std::string mp42ts(int argc, char **argv);
};

