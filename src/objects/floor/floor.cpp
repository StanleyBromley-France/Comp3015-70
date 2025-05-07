#include "floor.h"
#include "../../../helper/texture.h"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

Floor::Floor() : plane_(100.0f, 1, 5.0f) {}

void Floor::init()
{
	albedoTextures_.resize(1);

	auto diffuse = Texture::loadTexture("media/texture/floor/diffuse.jpg");
	albedoTextures_[0] = diffuse;
	normalTexture_ = Texture::loadTexture("media/texture/floor/normal.jpg");

	modelMatrix_ = glm::mat4(1.0f);

	modelMatrix_ = glm::translate(modelMatrix_, glm::vec3(0.0f, 0.0f, 0.0f));

	// material setup
	mat.Kd = vec3(0.7f, 0.7f, 0.7f);
	mat.Ks = vec3(0.9f, 0.9f, 0.9f);
	mat.Ka = vec3(0.2f, 0.2f, 0.2f);
	mat.Shininess = 25.f;
}

void Floor::update(float t) {}

void Floor::render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog)
{
	apply_textures(prog);

	applyMat(prog);

	apply_model_matrix(prog, view, projection);

	prog.setUniform("useSpecular", false);

	plane_.render();
}
