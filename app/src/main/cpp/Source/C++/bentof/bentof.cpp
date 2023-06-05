//
// Created by SJ on 2023/6/5.
//

#include "bentof.h"


extern "C" JNIEXPORT jstring
Java_com_et_bento_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
    string ret= Mp4Mux::getInstance().bboxh264("ssssssssssssssss");
    return env->NewStringUTF(ret.c_str());
}
