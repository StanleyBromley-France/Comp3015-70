#pragma once
#include "imgui/imgui.h"

namespace ImGuiUtils {

    ImVec2 GetGUIScreenCenteredPos(const ImVec2& elemSize);
    void SetWindowCenteredPos(const ImVec2& elemSize);
};