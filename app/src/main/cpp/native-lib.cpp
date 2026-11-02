#include <jni.h>
#include <atomic>
#include <string>

namespace {
    std::atomic_bool features[3] = {
        ATOMIC_VAR_INIT(false), // Barrett UI state
        ATOMIC_VAR_INIT(false), // RPG UI state
        ATOMIC_VAR_INIT(false)  // Speed UI state
    };

    const char* names[3] = {"Barrett", "RPG", "Speed"};
}

extern "C" JNIEXPORT void JNICALL
Java_com_raacheat_cloneinjector_NativeMenu_nativeSetFeature(
        JNIEnv*, jclass, jint id, jboolean enabled) {
    if (id >= 0 && id < 3) {
        features[id].store(enabled == JNI_TRUE);
    }
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_raacheat_cloneinjector_NativeMenu_nativeGetFeature(
        JNIEnv*, jclass, jint id) {
    if (id < 0 || id >= 3) return JNI_FALSE;
    return features[id].load() ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_raacheat_cloneinjector_NativeMenu_nativeGetStatus(
        JNIEnv* env, jclass) {
    std::string result;
    for (int i = 0; i < 3; ++i) {
        if (i) result += "\n";
        result += names[i];
        result += features[i].load() ? ": ON" : ": OFF";
    }
    return env->NewStringUTF(result.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_raacheat_cloneinjector_NativeMenu_nativeReset(
        JNIEnv*, jclass) {
    for (auto& feature : features) feature.store(false);
}
