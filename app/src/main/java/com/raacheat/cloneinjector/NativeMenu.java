package com.raacheat.cloneinjector;

public final class NativeMenu {
    static {
        System.loadLibrary("KXModsMenu");
    }

    private NativeMenu() {}

    public static native void nativeSetFeature(int id, boolean enabled);
    public static native boolean nativeGetFeature(int id);
    public static native String nativeGetStatus();
    public static native void nativeReset();
}
