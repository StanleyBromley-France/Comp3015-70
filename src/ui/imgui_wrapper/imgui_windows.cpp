#include "imgui_windows.h"
#include "imgui/imgui.h"

bool ImGuiWindows::BeginCustomGUIWindow(const char* title, const ImVec2& size, const ImVec2& position) {
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::SetNextWindowPos(position, ImGuiCond_Always);
    return ImGui::Begin(title, nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse
    );
}

void ImGuiWindows::EndGUIWindow() {
    ImGui::End();
}