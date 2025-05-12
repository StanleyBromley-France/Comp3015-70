#include "start_menu.h"
#include "../imgui_wrapper/imgui_windows.h"
#include "../imgui_wrapper/imgui_custom_widgets.h"
#include "../imgui_wrapper/imgui_utils.h"
#include "../../window/window.h"
#include "../imgui_wrapper/imgui/imgui.h"
#include "../../scene_management/scene_switcher.h"

#include "../../../scenebasic_uniform.h"
#include "../../../game_scene.h"

#include <memory>
#include "../../save_management/save_data_manager.h"

using namespace ImGui;
using namespace ImGuiWindows;
using namespace ImGuiUtils;

void StartMenu::init()
{
    windowSize.x = 250;
    windowSize.y = 350;
    centerPos = GetGUIScreenCenteredPos(windowSize);
    centerPos.x += 400;
    bestTime = SaveDataManager::Instance().Data().bestTime;
}

void StartMenu::update()
{
}

void StartMenu::render()
{
    if (BeginCustomGUIWindow("Menu", windowSize, centerPos)) {

        Text("FPS: %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate,
            ImGui::GetIO().Framerate
        );
        ImVec2 buttonSize(150, 50);

        ImGui::Text("current best time = %.3f", bestTime);

        SetWindowCenteredPos(buttonSize);
        if (ImGui::Button("Play", buttonSize)) {
            SceneSwitcher::Instance().QueueSceneSwitch(std::make_unique<GameScene>());
        }
        SetWindowCenteredPos(buttonSize);
        if (ImGui::Button("Quit", buttonSize)) {
            glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        }

        EndGUIWindow();
    }
}
