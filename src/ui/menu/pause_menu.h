#pragma once
#include "../ui_element.h"
#include "../imgui_wrapper/imgui/imgui.h"

class PauseMenu : public UIElement {
public:
    void init() override;
    void update() override;
    void render() override;
    bool forceOpen = false;
    float currentTime_;

private:
    bool isOpen;
    ImVec2 windowSize;
    ImVec2 centerPos;
};