#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Window {
public:
    static void createInstance();

    // removes copy constructor and assignment operator
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;



    static bool isKeyPressed(int key);
    static bool isKeyPressedOnce(int key);
    static void updateKeyState();

private:
    Window() = default;

    static Window* instance_;

    static std::vector<bool> previousKeyStates; // tracks previous key states
};