#include "imgui_utils.h"
#include "imgui/imgui.h"

ImVec2 ImGuiUtils::GetGUIScreenCenteredPos(const ImVec2& elemSize) {
    const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    return ImVec2(
        (displaySize.x - elemSize.x) * 0.5f,
        (displaySize.y - elemSize.y) * 0.5f
    );
}

void ImGuiUtils::SetWindowCenteredPos(const ImVec2& elemSize) {
    ImVec2 winSize = ImGui::GetWindowSize();
    ImVec2 cur = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cur.x + (winSize.x - elemSize.x) * 0.5f, cur.y));

}