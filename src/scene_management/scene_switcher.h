#pragma once
#include "../../helper/scene.h"
#include <memory>


class SceneSwitcher {
public:
    static SceneSwitcher& Instance();

    SceneSwitcher(const SceneSwitcher&) = delete;
    SceneSwitcher& operator=(const SceneSwitcher&) = delete;

    void QueueSceneSwitch(std::unique_ptr<Scene> newScene);
    void ApplyPendingSwitch();

    Scene* GetCurrentScene();

    bool IsCurrentSceneInitialised();
    void MarkSceneInitialised();

private:
    SceneSwitcher() = default;
    ~SceneSwitcher() = default;

    std::unique_ptr<Scene> currentScene;
    std::unique_ptr<Scene> pendingScene;
    bool isCurrentSceneInitialised = false;
};