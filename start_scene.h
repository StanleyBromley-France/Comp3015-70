#pragma once
#include "helper/scene.h"
#include <vector>
#include "src/ui/ui_element.h"
#include <memory>


class StartScene : public Scene 
{
public:
    void initScene() override;
    void update(float t) override;
    void render() override;
    void resize(int, int) override;

private:
    std::vector<std::unique_ptr<UIElement>> uiElements_;

};