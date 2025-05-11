#pragma once

#include "../../helper/glslprogram.h"
#include <memory>
#include "../../helper/objmesh.h"
#include <vector>

class SceneObject {
public:
    virtual void init() = 0;
    virtual void update(float t) = 0;
    virtual void render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) = 0;
    virtual void renderDepth(GLSLProgram& depthProg) = 0;

    const static int LIGHT_UNIT;

    void set_position(glm::vec3 position);
protected:
    std::unique_ptr<ObjMesh> mesh_;
    glm::mat4 modelMatrix_;
    std::vector<GLuint> albedoTextures_;
    GLuint normalTexture_;
    struct Material {
        glm::vec3 Kd, Ks, Ka;
        float Shininess;
    } mat;
    const static int MAX_TEXTURES;
    const static int ALBEDO_START;
    const static int NORMAL_START;

    glm::vec3 position_;

    void defualt_cleanup();
    void applyMat(GLSLProgram& prog);
    void apply_model_matrix(GLSLProgram& prog, const glm::mat4& view, const glm::mat4& projection);
    void apply_textures(GLSLProgram& prog);
};