#include <jni.h>
#include <string>

JNIEXPORT void JNICALL
Java_com_zxtcode_alogger_ALogger_println(JNIEnv *env, jobject instance, jint  priority_, jstring tag_,
                                         jstring msg_) {
    const char *tag = env->GetStringUTFChars(tag_, 0);
    const char *msg = env->GetStringUTFChars(msg_, 0);

    // TODO

    env->ReleaseStringUTFChars(tag_, tag);
    env->ReleaseStringUTFChars(msg_, msg);
}

extern "C"
jstring
Java_com_zxtcode_alogger_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
