#include "barrel.h"

#include <glm/ext/matrix_transform.hpp>
#include "../../../helper/texture.h"


using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

void Barrel::init()
{
    barrel_ = ObjMesh::load("media/model/barrel.obj");

    auto diffuse = Texture::loadTexture("media/texture/barrel/diffuse.png");
    albedoTextures_.resize(1);
    albedoTextures_[0] = diffuse;
    normalTexture_ = Texture::loadTexture("media/texture/barrel/normal.png");

    modelMatrix_ = mat4(1.0f);
    modelMatrix_ = glm::translate(modelMatrix_, position_);

    modelMatrix_ = glm::scale(modelMatrix_, vec3(5.f));

    update_collider_position(glm::vec2(position_.x, position_.z));
    set_collider_size(glm::vec2(3.f,3.f));
    update_collider_angle(0.f);
}

void Barrel::update(float t)
{
}

void Barrel::render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog)
{
    apply_textures(prog);

    applyMat(prog);

    apply_model_matrix(prog, view, projection);

    prog.setUniform("useSpecular", false);

    barrel_->render();
}

void Barrel::renderDepth(GLSLProgram& depthProg)
{
    depthProg.setUniform("model", modelMatrix_);
    barrel_->render();
}

void Barrel::on_collision(CollisionObject& other)
{
}

void Barrel::on_collision_once(CollisionObject& other)
{
}