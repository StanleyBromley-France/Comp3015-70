#include "scene_object.h"
#include "../../helper/glslprogram.h"
#include <string>

const int SceneObject::MAX_TEXTURES = 5;
const int SceneObject::ALBEDO_START = 0;
const int SceneObject::NORMAL_START = ALBEDO_START + MAX_TEXTURES;
const int SceneObject::LIGHT_UNIT = NORMAL_START + 1;

void SceneObject::set_position(glm::vec3 position)
{
    position_ = position;
}

void SceneObject::defualt_cleanup()
{
    for (GLuint tex : albedoTextures_) {
        if (tex != 0) {
            glDeleteTextures(1, &tex);
        }
    }
    albedoTextures_.clear();

    if (normalTexture_ != 0) {
        glDeleteTextures(1, &normalTexture_);
        normalTexture_ = 0;
    }

    mesh_.reset();
}

void SceneObject::applyMat(GLSLProgram& prog)
{
    prog.setUniform("Material.Kd", mat.Kd);
    prog.setUniform("Material.Ks", mat.Ks);
    prog.setUniform("Material.Ka", mat.Ka);
    prog.setUniform("Material.Shininess", mat.Shininess);
}

void SceneObject::apply_model_matrix(GLSLProgram& prog, const glm::mat4& view, const glm::mat4& projection)
{

    glm::mat4 mv = view * modelMatrix_;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("Model", modelMatrix_);

}

void SceneObject::apply_textures(GLSLProgram& prog)
{
    int N = static_cast<int>(albedoTextures_.size());

    for (int i = 0; i < N; ++i) {
        int unit = ALBEDO_START + i;
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, albedoTextures_[i]);

        std::string uniformName = std::string("albedoArray[")
            + std::to_string(i)
            + "]";
        prog.setUniform(uniformName.c_str(), unit);
    }

    glActiveTexture(GL_TEXTURE0 + NORMAL_START);
    glBindTexture(GL_TEXTURE_2D, normalTexture_);
    prog.setUniform("NormalTex", NORMAL_START);

    prog.setUniform("numTextures", N);
}
