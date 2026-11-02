#pragma once

#if defined(__GNUC__)
#define KOVAK_API __attribute__((visibility("default")))
#else
#define KOVAK_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum KovakFeature {
    KOVAK_FEATURE_BARRETT = 0,
    KOVAK_FEATURE_RPG = 1,
    KOVAK_FEATURE_SPEED = 2
};

// Call with a current GLES3 context. Returns 1 on success, 0 on failure.
KOVAK_API int KovakMenu_Init(void);
KOVAK_API void KovakMenu_Shutdown(void);

// Call on the GL/render thread.
KOVAK_API void KovakMenu_SetDisplaySize(float width, float height);
KOVAK_API void KovakMenu_Render(void);

// Forward single-touch input. x/y are in the same coordinate space as display size.
KOVAK_API void KovakMenu_Touch(float x, float y, int is_down);

// Read/write menu state. These functions do not alter game behavior by themselves.
KOVAK_API void KovakMenu_SetFeature(int feature, int enabled);
KOVAK_API int KovakMenu_GetFeature(int feature);
KOVAK_API int KovakMenu_IsInitialized(void);

#ifdef __cplusplus
}
#endif
