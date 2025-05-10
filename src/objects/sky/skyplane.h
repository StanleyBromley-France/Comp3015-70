#pragma once

#include "../../../helper/glslprogram.h"
#include "../scene_object.h"

class SkyPlane : SceneObject {
public:
    void init() override;
    void update(float t) override;
    void render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) override;
    void renderDepth(GLSLProgram& depthProg) override;

private:
    GLuint handle[2];
    GLuint quad;
    GLuint noiseTex;
    GLSLProgram prog;
};