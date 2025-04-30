#pragma once
#include <GLFW/glfw3.h>

namespace ImGuiCore {

    // Core lifecycle
    void Init(GLFWwindow* window);
    void BeginFrame();
    void EndFrame();
    void Shutdown();

    // Global styling
    void SetStyleColorsDark();
    void SetStyleColorsLight();
    void SetStyleColorsClassic();
};