#pragma once
#include "../../helper/scene.h"


class SceneSwitcher {
public:
    static SceneSwitcher& Instance();

    SceneSwitcher(const SceneSwitcher&) = delete;
    SceneSwitcher& operator=(const SceneSwitcher&) = delete;

    void SwitchScene(const Scene& scene);

    const Scene& GetCurrentScene() const;

    bool IsCurrentSceneInitialised() const;
    void MarkSceneInitialised();

private:
    SceneSwitcher() = default;
    ~SceneSwitcher() = default;

    const Scene* currentScene = nullptr;
    bool isCurrentSceneInitialised = false;
};