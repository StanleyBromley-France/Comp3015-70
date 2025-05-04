#include "floor.h"
#include "../../../helper/texture.h"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

Floor::Floor() : plane_(100.0f, 1, 5.0f) {}

void Floor::init(GLSLProgram& prog)
{
	prog_ = &prog;

	diffuse_= Texture::loadTexture("media/texture/floor/diffuse.jpg");
	normal_ = Texture::loadTexture("media/texture/floor/normal.jpg");

	model_ = glm::mat4(1.0f);

	model_ = glm::translate(model_, glm::vec3(0.0f, 0.0f, 0.0f));


}

void Floor::update(float t)
{

}

void Floor::render(const glm::mat4& view, const glm::mat4& projection)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse_);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diffuse_);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normal_);

	prog_->setUniform("Material.Kd", glm::vec3(0.7f, 0.7f, 0.7f));
	prog_->setUniform("Material.Ks", glm::vec3(0.9f, 0.9f, 0.9f));
	prog_->setUniform("Material.Ka", glm::vec3(0.2f, 0.2f, 0.2f));
	prog_->setUniform("Material.Shininess", 25.f);

	glm::mat4 mv = view * model_;
	prog_->setUniform("ModelViewMatrix", mv);
	prog_->setUniform("NormalMatrix", glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
	prog_->setUniform("MVP", projection * mv);

	prog_->setUniform("useSpecular", false);


	plane_.render();
}
