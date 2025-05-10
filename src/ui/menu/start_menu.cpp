#include "start_menu.h"
#include "../imgui_wrapper/imgui_windows.h"
#include "../imgui_wrapper/imgui_custom_widgets.h"
#include "../imgui_wrapper/imgui_utils.h"
#include "../../window/window.h"
#include "../imgui_wrapper/imgui/imgui.h"
#include "../../scene_management/scene_switcher.h"

#include "../../../scenebasic_uniform.h"
#include <memory>

using namespace ImGui;
using namespace ImGuiWindows;
using namespace ImGuiUtils;

void StartMenu::init()
{
    windowSize.x = 400;
    windowSize.y = 600;
    centerPos = GetGUIScreenCenteredPos(windowSize);
}

void StartMenu::update()
{
}

void StartMenu::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (BeginCustomGUIWindow("Start SC", windowSize, centerPos)) {

        Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate,
            ImGui::GetIO().Framerate
        );
        ImVec2 buttonSize(150, 50);


        SetWindowCenteredPos(buttonSize);
        if (ImGui::Button("Go", buttonSize)) {
            SceneSwitcher::Instance().QueueSceneSwitch(std::make_unique<SceneBasic_Uniform>());
        }

        static bool enableFeature = true;
        ImGui::Checkbox("Enable Feature", &enableFeature);

        static float volume = 0.5f;
        ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
        SetWindowCenteredPos(buttonSize);
        if (ImGui::Button("Quit", buttonSize)) {
            glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        }

        EndGUIWindow();
    }
}
