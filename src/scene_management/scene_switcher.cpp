#include "scene_switcher.h"
#include "../../helper/scene.h"
#include <cassert>

SceneSwitcher& SceneSwitcher::Instance() {
    static SceneSwitcher instance;
    return instance;
}

void SceneSwitcher::SwitchScene(const Scene& scene) {
    currentScene = &scene;
    isCurrentSceneInitialised = false;
}

const Scene& SceneSwitcher::GetCurrentScene() const {
    assert(currentScene != nullptr && "No current scene set!");
    return *currentScene;
}

bool SceneSwitcher::IsCurrentSceneInitialised() const {
    return isCurrentSceneInitialised;
}

void SceneSwitcher::MarkSceneInitialised() {
    isCurrentSceneInitialised = true;
}