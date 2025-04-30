#pragma once
#include "imgui/imgui.h"

namespace ImGuiWindows {

    bool BeginCustomGUIWindow(const char* title, const ImVec2& size, const ImVec2& position);
    void EndGUIWindow();
};