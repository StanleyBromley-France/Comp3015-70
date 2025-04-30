#include "menu.h"
#include "../imgui_wrapper/imgui_windows.h"
#include "../imgui_wrapper/imgui_custom_widgets.h"
#include "../imgui_wrapper/imgui_utils.h"
#include "../../window/window.h"
#include "../imgui_wrapper/imgui/imgui.h"

#include <GLFW/glfw3.h>

using namespace ImGuiWindows;
using namespace ImGuiWidgets;
using namespace ImGuiUtils;

void Menu::init() 
{
    windowSize.x = 400;
    windowSize.y = 200;
    centerPos = GetGUIScreenCenteredPos(windowSize);
    isOpen = false;

    Window::createInstance();
}

void Menu::update() 
{
    if (Window::isKeyPressedOnce(GLFW_KEY_P)) {
        isOpen = !isOpen;
    }
};

void Menu::render() 
{
    if (!isOpen)
        return;

    if (BeginCustomGUIWindow("Settings", windowSize, centerPos)) {
        
        Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate,
            ImGui::GetIO().Framerate
        );

        static bool enableFeature = true;
        Checkbox("Enable Feature", &enableFeature);

        static float volume = 0.5f;
        SliderFloat("Volume", &volume, 0.0f, 1.0f);
        ImVec2 buttonSize(150, 50);
        SetWindowCenteredPos(buttonSize);
        if (Button("Quit", buttonSize)) {
            glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        }

        EndGUIWindow();
    }
}