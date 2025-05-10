#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include "helper/glslprogram.h"


#include "src/global_settings/global_settings_ubo/global_settings_ubo.h"

#include "src/objects/skybox/scene_skybox.h"
#include "src/global_settings/lights/particle_spawner/particle_spawner.h"
#include "src/objects/scene_object.h"
#include "src/objects/light_object.h"

#include "src/objects/uploader_object.h"
#include "src/ui/ui_element.h"
#include <memory>
#include <vector>
#include "src/objects/particle_object.h"
#include "src/debug/gs_to_rgba_converter.h"
#include "src/objects/sky/skyplane.h"

class SceneBasic_Uniform : public Scene
{
    std::vector<std::shared_ptr<SceneObject>> complexObjs_;
    std::vector<std::shared_ptr<ParticleObject>> particleObjs_;
    std::vector<std::shared_ptr<LightObject>> lightObjs_;

    std::vector<std::shared_ptr<UploaderObject>> uploaderObjs_;

    std::vector<std::unique_ptr<UIElement>> uiElements_;

    GLSLProgram complexProg_, skyboxProg_, particleProg_, depthProg_, cloudProg_;
    SceneSkybox skybox_;

    SkyPlane clouds;

    GlobalSettingsUBO globalSettings;

    GrayscaleToRGBAConverter converter;

    void set_matrices(GLSLProgram& prog);
    void compile_shaders();
    void init_ui();
    void draw_shadow_maps();
    void draw_scene();
    void draw_ui();

public:
    SceneBasic_Uniform();

    void initScene() override;
    void update( float t ) override;
    void render() override;
    void resize(int, int) override;
};

#endif // SCENEBASIC_UNIFORM_H
