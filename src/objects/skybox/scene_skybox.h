#pragma once

#include "../scene_object.h"
#include "../../../helper/skybox.h"

class SceneSkybox : public SceneObject 
{
public:
    SceneSkybox();
    ~SceneSkybox();
    void init() override;
    void update(float t) override;
    void render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) override;
    void renderDepth(GLSLProgram& prog) override;
private:
    GLSLProgram* prog_;
    SkyBox skybox_;
    GLuint tex_;
};