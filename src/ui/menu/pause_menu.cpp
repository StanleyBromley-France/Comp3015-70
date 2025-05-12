#include "pause_menu.h"
#include "../imgui_wrapper/imgui_windows.h"
#include "../imgui_wrapper/imgui_custom_widgets.h"
#include "../imgui_wrapper/imgui_utils.h"
#include "../../window/window.h"
#include "../imgui_wrapper/imgui/imgui.h"

#include <GLFW/glfw3.h>
#include "../../scene_management/scene_switcher.h"
#include "../../../start_scene.h"
#include "../../../sceneBasic_uniform.h"
#include "../../save_management/save_data_manager.h"

#include <string>
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
    if (!isOpen && !forceOpen)
        return;

    if (BeginCustomGUIWindow("Settings", windowSize, centerPos)) {
        
        Text("FPS: %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate,
            ImGui::GetIO().Framerate
        );

        ImGui::Text("current time = %.3f", currentTime_);

        ImVec2 buttonSize(150, 50);

        SetWindowCenteredPos(buttonSize);
        if (ImGui::Button("Go to car select", buttonSize)) {
            SceneSwitcher::Instance().QueueSceneSwitch(std::make_unique<SceneBasic_Uniform>());
        }

        SetWindowCenteredPos(buttonSize);
        if (Button("Quit", buttonSize)) {
            glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        }

        EndGUIWindow();
    }
}