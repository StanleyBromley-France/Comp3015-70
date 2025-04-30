#pragma once
#include "imgui/imgui.h"

namespace ImGuiWidgets {

    void Text(const char* fmt, ...) IM_FMTARGS(1);
    bool Button(const char* label, const ImVec2& size = ImVec2(0, 0));
    bool Checkbox(const char* label, bool* value);
    bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f");
    bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d");
    bool InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0);
};