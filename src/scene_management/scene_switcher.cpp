#include "scene_switcher.h"
#include "../../helper/scene.h"
#include <cassert>

SceneSwitcher& SceneSwitcher::Instance() {
    static SceneSwitcher instance;
    return instance;
}

void SceneSwitcher::QueueSceneSwitch(std::unique_ptr<Scene> newScene) {
    pendingScene = std::move(newScene);
}

void SceneSwitcher::ApplyPendingSwitch() {
    if (pendingScene) {
        currentScene = std::move(pendingScene);
        isCurrentSceneInitialised = false;
    }
}

Scene* SceneSwitcher::GetCurrentScene() {
    assert(currentScene != nullptr && "No current scene set!");
    return currentScene.get();
}

bool SceneSwitcher::IsCurrentSceneInitialised() {
    return isCurrentSceneInitialised;
}

void SceneSwitcher::MarkSceneInitialised() {
    isCurrentSceneInitialised = true;
}