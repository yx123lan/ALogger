#include <jni.h>
#include <string>
#include "LogManager.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_zxtcode_alogger_ALogger_println(JNIEnv *env, jobject instance, jint  priority_, jstring tag_,
                                         jstring msg_) {
    const char *tag = env->GetStringUTFChars(tag_, 0);
    const char *msg = env->GetStringUTFChars(msg_, 0);

    LogManager& logManager = LogManager::getInstance();
    logManager.println(tag, msg);

    env->ReleaseStringUTFChars(tag_, tag);
    env->ReleaseStringUTFChars(msg_, msg);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zxtcode_alogger_ALogger_init(JNIEnv *env, jobject instance, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);
    // TODO
    LogManager::getInstance().init(path);

    env->ReleaseStringUTFChars(path_, path);
}

extern "C"
jstring
Java_com_zxtcode_alogger_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
