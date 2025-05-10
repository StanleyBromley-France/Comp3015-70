#include "pause_menu.h"
#include "../imgui_wrapper/imgui_windows.h"
#include "../imgui_wrapper/imgui_custom_widgets.h"
#include "../imgui_wrapper/imgui_utils.h"
#include "../../window/window.h"
#include "../imgui_wrapper/imgui/imgui.h"

#include <GLFW/glfw3.h>
#include "../../scene_management/scene_switcher.h"
#include "../../../start_scene.h"

using namespace ImGuiWindows;
using namespace ImGuiWidgets;
using namespace ImGuiUtils;

void PauseMenu::init() 
{
    windowSize.x = 400;
    windowSize.y = 200;
    centerPos = GetGUIScreenCenteredPos(windowSize);
    isOpen = false;
}

void PauseMenu::update() 
{
    if (Input::isKeyPressedOnce(GLFW_KEY_P)) {
        isOpen = !isOpen;
    }
};

void PauseMenu::render() 
{
    if (!isOpen)
        return;

    if (BeginCustomGUIWindow("Settings", windowSize, centerPos)) {
        
        Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate,
            ImGui::GetIO().Framerate
        );
        ImVec2 buttonSize(150, 50);

        SetWindowCenteredPos(buttonSize);
        if (ImGui::Button("Go", buttonSize)) {
            SceneSwitcher::Instance().QueueSceneSwitch(std::make_unique<StartScene>());
        }

        static bool enableFeature = true;
        Checkbox("Enable Feature", &enableFeature);

        static float volume = 0.5f;
        SliderFloat("Volume", &volume, 0.0f, 1.0f);
        SetWindowCenteredPos(buttonSize);
        if (Button("Quit", buttonSize)) {
            glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        }

        EndGUIWindow();
    }
}