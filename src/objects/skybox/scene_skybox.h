#pragma once

#include "../object.h"
#include "../../../helper/skybox.h"

class SceneSkybox : Object 
{
public:
    SceneSkybox();
    void init(GLSLProgram& prog) override;
    void update(float t) override;
    void render(const glm::mat4& view, const glm::mat4& projection) override;
private:
    GLSLProgram* prog_;
    SkyBox skybox_;
    GLuint tex_;
    glm::mat4 model_;
};