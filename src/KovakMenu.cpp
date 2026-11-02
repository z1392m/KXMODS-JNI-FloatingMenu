#include "KovakMenu.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include <GLES3/gl3.h>
#include <algorithm>

namespace {
bool g_initialized = false;
bool g_features[3] = {false, false, false};
float g_width = 1080.0f;
float g_height = 1920.0f;
bool g_touch_down = false;

const char* FeatureName(int id) {
    switch (id) {
        case KOVAK_FEATURE_BARRETT: return "ENABLE BARRETT";
        case KOVAK_FEATURE_RPG:     return "ENABLE RPG";
        case KOVAK_FEATURE_SPEED:   return "ENABLE SPEED";
        default:                    return "UNKNOWN";
    }
}

void DrawMenu() {
    ImGuiIO& io = ImGui::GetIO();
    const float scale = std::clamp(std::min(g_width / 420.0f, g_height / 760.0f),
                                   0.75f, 1.35f);
    ImGui::SetNextWindowSize(ImVec2(330.0f * scale, 0.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(g_width * 0.5f, g_height * 0.18f),
                            ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 14.0f));

    if (ImGui::Begin("KOVAK // NATIVE MENU")) {
        ImGui::TextDisabled("ANDROID ARM64  |  IMGUI");
        ImGui::Separator();
        ImGui::Spacing();

        for (int i = 0; i < 3; ++i) {
            ImGui::PushID(i);
            ImGui::Checkbox(FeatureName(i), &g_features[i]);
            ImGui::PopID();
            ImGui::Spacing();
        }

        ImGui::Separator();
        ImGui::TextDisabled("UI state only — connect your own game logic.");
    }
    ImGui::End();

    ImGui::PopStyleVar(3);
}
} // namespace

extern "C" {

int KovakMenu_Init(void) {
    if (g_initialized) return 1;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding = 7.0f;

    if (!ImGui_ImplOpenGL3_Init("#version 300 es")) {
        ImGui::DestroyContext();
        return 0;
    }

    g_initialized = true;
    return 1;
}

void KovakMenu_Shutdown(void) {
    if (!g_initialized) return;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    g_initialized = false;
}

void KovakMenu_SetDisplaySize(float width, float height) {
    g_width = std::max(1.0f, width);
    g_height = std::max(1.0f, height);
    if (g_initialized) {
        ImGui::GetIO().DisplaySize = ImVec2(g_width, g_height);
    }
}

void KovakMenu_Touch(float x, float y, int is_down) {
    if (!g_initialized) return;
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(x, y);
    const bool down = is_down != 0;
    if (down != g_touch_down) {
        io.AddMouseButtonEvent(0, down);
        g_touch_down = down;
    }
}

void KovakMenu_Render(void) {
    if (!g_initialized) return;

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(g_width, g_height);
    io.DeltaTime = (io.DeltaTime > 0.0f) ? io.DeltaTime : (1.0f / 60.0f);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    DrawMenu();
    ImGui::Render();

    glViewport(0, 0, static_cast<GLsizei>(g_width),
               static_cast<GLsizei>(g_height));
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void KovakMenu_SetFeature(int feature, int enabled) {
    if (feature < 0 || feature >= 3) return;
    g_features[feature] = enabled != 0;
}

int KovakMenu_GetFeature(int feature) {
    if (feature < 0 || feature >= 3) return 0;
    return g_features[feature] ? 1 : 0;
}

int KovakMenu_IsInitialized(void) {
    return g_initialized ? 1 : 0;
}

} // extern "C"
