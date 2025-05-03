#include "scene_skybox.h"
#include "../../../helper/texture.h"
#include <glm/ext/matrix_transform.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneSkybox::SceneSkybox() : skybox_(100.0f){}

void SceneSkybox::init(GLSLProgram& prog)
{
	prog_ = &prog;
	tex_ = Texture::loadCubeMap("media/texture/yokohama/yokohama", ".jpg");

	model_ = mat4(1.0f);
	model_ = glm::translate(model_, vec3(0.0f, 11.0f, 0.0f));
}

void SceneSkybox::update(float t)
{
}

void SceneSkybox::render(const glm::mat4& view, const glm::mat4& projection)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_);

	mat4 mv = view * model_;
	prog_->setUniform("ModelViewMatrix", mv);
	prog_->setUniform("NormalMatrix", glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
	prog_->setUniform("MVP", projection * mv);

	skybox_.render();
}
