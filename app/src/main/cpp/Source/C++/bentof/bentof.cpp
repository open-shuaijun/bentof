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
    string out = "/sdcard/out1.mp4";

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

extern "C" JNIEXPORT jstring
Java_com_et_bento_MainActivity_mp42hls(JNIEnv *env, jobject /* this */) {
    string pro = "hls";
    string src = "/sdcard/out.mp4";
    string n = "--segment-filename-template";
    string n1 = "/sdcard/ssssaaaaa-%d.ts";
    string a = "--audio-track-id";
    string a1 = "0";
    string s = "--segment-url-template";
    string s1 = "ssssaaaaa-%d.ts";
    string o = "--index-filename";
    string o1 = "/sdcard/aaaa.m3u8";
    string end="end";


    // ./mp42hls ou.mp4 --index-filename aaaa.m3u8   --segment-url-template ssssaaaaa-%d.ts
    // --segment-filename-template ssssaaaaa-%d.ts --audio-track-id 0


    char* params[] = {const_cast<char *>(pro.c_str()),
                      const_cast<char *>(src.c_str()),
                      const_cast<char *>(n.c_str()),
                      const_cast<char *>(n1.c_str()),
                      const_cast<char *>(a.c_str()),
                      const_cast<char *>(a1.c_str()),
                      const_cast<char *>(s.c_str()),
                      const_cast<char *>(s1.c_str()),
                      const_cast<char *>(o.c_str()),
                      const_cast<char *>(o1.c_str()),
                      const_cast<char *>(end.c_str())
    };
    char** pp = params;
//    __android_log_print(ANDROID_LOG_DEBUG, "ddd", "参数='%s'", *++pp);
//    __android_log_print(ANDROID_LOG_DEBUG, "ddd", "参数='%s'", *++pp);
//    __android_log_print(ANDROID_LOG_DEBUG, "ddd", "参数='%s'", *++pp);
// mp42hls ou.mp4
    string ret= Mp42Hls::getInstance().mp42hls(9, pp);
    __android_log_print(ANDROID_LOG_DEBUG, "ddd", "Hls:%s", ret.c_str());
    return env->NewStringUTF(ret.c_str());
}


extern "C" JNIEXPORT jstring
Java_com_et_bento_MainActivity_mp42ts(JNIEnv *env, jobject /* this */) {
    string pro = "ts";
    string src = "/sdcard/out.mp4";
    string out = "/sdcard/out2.ts";
    char* params[] = {const_cast<char *>(pro.c_str()), const_cast<char *>(src.c_str()),const_cast<char *>(out.c_str()),
                      const_cast<char *>(pro.c_str())};
    char** pp = params;
    string ret= Mp42Ts::getInstance().mp42ts(3, pp);
    __android_log_print(ANDROID_LOG_DEBUG, "ddd", "Ts:%s", ret.c_str());
    return env->NewStringUTF(ret.c_str());
}

extern "C" JNIEXPORT void
Java_com_et_bento_MainActivity_openFile(JNIEnv *env, jobject /* this */) {
//    const char *src = "/sdcard/ou.mp4";
//    FILE *ff = fopen(src, "wb+");
//    __android_log_print(ANDROID_LOG_DEBUG, "ddd", "bbbbbbb=%p", ff);
}
