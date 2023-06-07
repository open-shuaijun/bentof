//
// Created by SJ on 2023/6/5.
//

#include "bentof.h"


using namespace std;

extern "C" JNIEXPORT jstring
Java_com_et_bentof_Bentof_mp4mux(JNIEnv *env, jobject /* this */, jstring h264_path,
                                 jstring mp4_path) {
    const char *c_h264_path = env->GetStringUTFChars(h264_path, JNI_FALSE);
    const char *c_mp4_path = env->GetStringUTFChars(mp4_path, JNI_FALSE);
    char c_track[] = "--track\0";
    char c_pro[] = "mux\0";

    char *params[] = {const_cast<char *>(c_pro),
                      c_track,
                      const_cast<char *>(c_h264_path),
                      const_cast<char *>(c_mp4_path),
                      const_cast<char *>(c_pro)};
    char **pp = params;
    string ret = Mp4Mux::getInstance().bboxh264(pp);
    if ("success" != ret) {
        __android_log_print(ANDROID_LOG_ERROR, "TAG", "Mux转换报错了:%s", ret.c_str());
    } else {
        __android_log_print(ANDROID_LOG_DEBUG, "TAG", "Mux转换成功了:%s", ret.c_str());
    }
    env->ReleaseStringUTFChars(h264_path, c_h264_path);
    env->ReleaseStringUTFChars(mp4_path, c_mp4_path);
    return env->NewStringUTF(ret.c_str());
}

extern "C" JNIEXPORT jstring
Java_com_et_bentof_Bentof_mp42hls(JNIEnv *env, jobject /* this */, jstring mp4_src_path,
                                  jstring out_directory, jstring out_hls_name,
                                  jstring out_ts_name) {
    const char *c_mp4_src_path = env->GetStringUTFChars(mp4_src_path, JNI_FALSE);
    const char *c_out_directory = env->GetStringUTFChars(out_directory, JNI_FALSE);
    const char *c_out_hls_name = env->GetStringUTFChars(out_hls_name, JNI_FALSE);
    const char *c_out_ts_name = env->GetStringUTFChars(out_ts_name, JNI_FALSE);

    string pro = "hls";
    string cmd_segment_filename_template = "--segment-filename-template";
    string segment_filename_template = string(c_out_directory) + string(c_out_ts_name);
    string audio_track_id = "--audio-track-id";
    string audio_less = "0";
    string cmd_segment_url_template = "--segment-url-template";
    string cmd_m3u8_index_filename = "--index-filename";
    string m3u8_file_path = string(c_out_directory) + string(c_out_hls_name);

    char *params[] = {const_cast<char *>(pro.c_str()),
                      const_cast<char *>(c_mp4_src_path),
                      const_cast<char *>(cmd_segment_filename_template.c_str()),
                      const_cast<char *>(segment_filename_template.c_str()),
                      const_cast<char *>(audio_track_id.c_str()),
                      const_cast<char *>(audio_less.c_str()),
                      const_cast<char *>(cmd_segment_url_template.c_str()),
                      const_cast<char *>(segment_filename_template.c_str()),
                      const_cast<char *>(cmd_m3u8_index_filename.c_str()),
                      const_cast<char *>(m3u8_file_path.c_str()),
                      const_cast<char *>(pro.c_str())
    };
    char **pp = params;
    string ret = Mp42Hls::getInstance().mp42hls(9, pp);
    __android_log_print(ANDROID_LOG_DEBUG, "Mp42Hls:::", "Hls:%s", ret.c_str());
    env->ReleaseStringUTFChars(mp4_src_path, c_mp4_src_path);
    env->ReleaseStringUTFChars(out_directory, c_out_directory);
    env->ReleaseStringUTFChars(out_hls_name, c_out_hls_name);
    env->ReleaseStringUTFChars(out_ts_name, c_out_ts_name);
    return env->NewStringUTF(ret.c_str());
}


extern "C" JNIEXPORT jstring
Java_com_et_bentof_Bentof_mp42ts(JNIEnv *env, jobject /* this */, jstring input_mp4, jstring out_ts) {
    const char* c_input_mp4 = env->GetStringUTFChars(input_mp4, JNI_FALSE);
    const char* c_out_ts = env->GetStringUTFChars(out_ts, JNI_FALSE);
    string pro = "ts";
    char *params[] = {const_cast<char *>(pro.c_str()),
                      const_cast<char *>(c_input_mp4),
                      const_cast<char *>(c_out_ts),
                      const_cast<char *>(pro.c_str())};
    char **pp = params;
    string ret = Mp42Ts::getInstance().mp42ts(3, pp);
    __android_log_print(ANDROID_LOG_DEBUG, "TAG", "视频转换TS完成:%s", ret.c_str());
    env->ReleaseStringUTFChars(input_mp4, c_input_mp4);
    env->ReleaseStringUTFChars(out_ts, c_out_ts);
    return env->NewStringUTF(ret.c_str());
}
