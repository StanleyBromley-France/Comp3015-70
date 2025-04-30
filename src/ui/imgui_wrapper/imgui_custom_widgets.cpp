#include "imgui_custom_widgets.h"
#include "imgui/imgui.h"
#include <vadefs.h>
#include <cstdarg>


void ImGuiWidgets::Text(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::TextV(fmt, args);
    va_end(args);
}

bool ImGuiWidgets::Button(const char* label, const ImVec2& size) {
    return ImGui::Button(label, size);
}

bool ImGuiWidgets::Checkbox(const char* label, bool* value) {
    return ImGui::Checkbox(label, value);
}

bool ImGuiWidgets::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format) {
    return ImGui::SliderFloat(label, v, v_min, v_max, format);
}

bool ImGuiWidgets::SliderInt(const char* label, int* v, int v_min, int v_max, const char* format) {
    return ImGui::SliderInt(label, v, v_min, v_max, format);
}

bool ImGuiWidgets::InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags) {
    return ImGui::InputText(label, buf, buf_size, flags);
}