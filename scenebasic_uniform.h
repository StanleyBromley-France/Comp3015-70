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
#include "src/objects/scene_object.h"

class SceneBasic_Uniform : public Scene
{
    std::vector<std::unique_ptr<SceneObject>> objects_;

    GLSLProgram prog_, prog2_, skyboxProg_, spawnerProg_;
    SceneSkybox skybox_;

    GLuint particleTex_;
    ParticleSpawner spawner_;

    GlobalSettingsUBO globalSettings;
    spotlight spotlight_;
    
    float t_prev_;
    float angle_;

    void set_matrices(GLSLProgram& prog);
    void compile_shaders();
    void init_ui();
    void draw_scene();


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
