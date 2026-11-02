#include <jni.h>
#include <atomic>
#include <string>
#include <sstream>

namespace {
    // UI state only. These values do not alter game memory or gameplay.
    std::atomic_bool g_features[3] = {
        false, false, false
    };
    std::atomic_bool g_initialized{false};

    bool validId(jint id) {
        return id >= 0 && id < 3;
    }

    const char* featureName(int id) {
        switch (id) {
            case 0: return "Barrett";
            case 1: return "RPG";
            case 2: return "Speed";
            default: return "Unknown";
        }
    }

    jstring toJString(JNIEnv* env, const std::string& value) {
        return env->NewStringUTF(value.c_str());
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_raacheat_cloneinjector_NativeMenu_nativeInit(JNIEnv*, jclass) {
    g_initialized.store(true);
    return JNI_TRUE;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_raacheat_cloneinjector_NativeMenu_nativeSetFeature(
        JNIEnv*, jclass, jint id, jboolean enabled) {
    if (!validId(id)) return;
    g_features[id].store(enabled == JNI_TRUE);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_raacheat_cloneinjector_NativeMenu_nativeGetFeature(
        JNIEnv*, jclass, jint id) {
    if (!validId(id)) return JNI_FALSE;
    return g_features[id].load() ? JNI_TRUE : JNI_FALSE;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_raacheat_cloneinjector_NativeMenu_nativeGetStatus(
        JNIEnv* env, jclass) {
    std::ostringstream out;
    out << "KXMODS JNI 1.2 | ";
    for (int i = 0; i < 3; ++i) {
        if (i) out << " | ";
        out << featureName(i) << "="
            << (g_features[i].load() ? "ON" : "OFF");
    }
    return toJString(env, out.str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_raacheat_cloneinjector_NativeMenu_nativeReset(
        JNIEnv*, jclass) {
    for (auto& feature : g_features) feature.store(false);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_raacheat_cloneinjector_NativeMenu_nativeRelease(
        JNIEnv*, jclass) {
    for (auto& feature : g_features) feature.store(false);
    g_initialized.store(false);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_raacheat_cloneinjector_NativeMenu_nativeGetVersion(
        JNIEnv* env, jclass) {
    return toJString(env, "KXMODS JNI 1.2");
}
