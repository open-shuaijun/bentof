//
// Created by SJ on 2023/6/5.
//

#include "bentof.h"
using namespace std;

extern "C" JNIEXPORT jstring
Java_com_et_bento_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
    string pro = "mux";
    string cmd = "--track";
    string src = "/sdcard/test.h264";
    string out = "/sdcard/out.mp4";

    char* params[] = {const_cast<char *>(pro.c_str()), const_cast<char *>(cmd.c_str()), const_cast<char *>(src.c_str()),
                      const_cast<char *>(out.c_str()), const_cast<char *>(pro.c_str())};
    char** pp = params;
//    __android_log_print(ANDROID_LOG_DEBUG, "ddd", "参数='%s'", *++pp);
//    __android_log_print(ANDROID_LOG_DEBUG, "ddd", "参数='%s'", *++pp);
//    __android_log_print(ANDROID_LOG_DEBUG, "ddd", "参数='%s'", *++pp);
    string ret= Mp4Mux::getInstance().bboxh264(pp);
    __android_log_print(ANDROID_LOG_DEBUG, "ddd", "ddddddddddddddddddddd:%s", ret.c_str());
    return env->NewStringUTF(ret.c_str());
}
