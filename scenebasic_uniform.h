#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/skybox.h"

#include "src/objects/cars/showcase_car/showcase_car.h"
#include "src/objects/floor/floor.h"

#include "src/global_settings/lights/spotlight/spotlight.h"
#include "src/global_settings/global_settings_ubo/global_settings_ubo.h"

#include "src/ui/menu/menu.h"

class SceneBasic_Uniform : public Scene
{
    Plane platform_;
    Floor floor_;
    showcase_car showcase_car_;
    SkyBox skybox_;

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
