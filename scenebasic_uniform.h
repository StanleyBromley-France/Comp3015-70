#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include "helper/glslprogram.h"

#include "src/objects/cars/showcase_car/showcase_car.h"
#include "src/objects/floor/floor.h"

#include "src/global_settings/lights/spotlight/spotlight.h"
#include "src/global_settings/global_settings_ubo/global_settings_ubo.h"

#include "src/ui/menu/menu.h"
#include "src/objects/skybox/scene_skybox.h"
#include "src/global_settings/lights/particle_spawner/particle_spawner.h"

class SceneBasic_Uniform : public Scene
{
    Floor floor_;
    showcase_car showcase_car_;
    SceneSkybox skybox_;

    GLuint particleTex_;
    ParticleSpawner spawner_;
    GLSLProgram spawnerProg_;

    GlobalSettingsUBO globalSettings;
    spotlight spotlight_;
    
    GLSLProgram prog_;
    GLSLProgram prog2_;
    GLSLProgram skyboxProg_;

    float t_prev_;
    float angle_;

    void set_matrices(GLSLProgram& prog);
    void compile();
    
    void draw_scene();

    // customisation settings

    bool toon_shading_ = false;
    bool hdr_mode_ = true;
    bool normal_mode_ = true;

    // ui

    Menu menu;

public:
    SceneBasic_Uniform();

    void initScene() override;
    void update( float t ) override;
    void render() override;
    void resize(int, int) override;
};

#endif // SCENEBASIC_UNIFORM_H
