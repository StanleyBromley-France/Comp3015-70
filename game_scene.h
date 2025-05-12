#pragma once

#include "helper/scene.h"
#include <vector>
#include <memory>
#include "src/objects/scene_object.h"
#include "src/objects/particle_object.h"
#include "src/objects/light_object.h"
#include "src/objects/uploader_object.h"
#include "src/ui/ui_element.h"
#include "src/collision/rendering/collision_debug_renderer.h"
#include "src/collision/collision_object/collision_object.h"
#include "src/collision/collision_manager/collision_manager.h"

#include "src/cameras/game_cam/game_cam.h"
#include "src/objects/cars/game_car/game_car.h"
#include "src/objects/interactable/game_checkpoint.h"
#include "src/objects/skybox/scene_skybox.h"
#include "src/objects/sky/skyplane.h"

class GameScene : public Scene 
{
public:
    GameScene();
    ~GameScene();
    void initScene() override;
    void update(float t) override;
    void render() override;
    void resize(int, int) override;

    void draw_scene();
    void draw_shadow_maps();

    void compile_shaders();

private:

    GameCam camera;
    std::shared_ptr<GameCar> car_;
    std::vector<std::shared_ptr<SceneObject>> complexObjs_;
    std::vector<std::shared_ptr<ParticleObject>> particleObjs_;
    std::vector<std::shared_ptr<LightObject>> lightObjs_;

    std::vector<std::shared_ptr<UploaderObject>> uploaderObjs_;

    std::vector<std::shared_ptr<CollisionObject>> collisionObjs_;
    std::vector<std::shared_ptr<Checkpoint>> checkpointObjs_;
    
    SceneSkybox skybox_;
    SkyPlane clouds;


    std::vector<std::unique_ptr<UIElement>> uiElements_;

    GlobalSettingsUBO globalSettings;


    GLSLProgram complexProg_, skyboxProg_, particleProg_, depthProg_, cloudProg_, collisionProg_;

    CollisionManager collisionManager;
    CollisionDebugRenderer collisionDebugRenderer;
};