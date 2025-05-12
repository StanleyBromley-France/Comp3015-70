#pragma once
#include "../ui_element.h"
#include "../imgui_wrapper/imgui/imgui.h"

class StartMenu : public UIElement {
public:
    void init() override;
    void update() override;
    void render() override;
private:
    ImVec2 windowSize;
    ImVec2 centerPos;
    float bestTime;
};