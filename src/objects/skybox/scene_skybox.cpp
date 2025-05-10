#include "scene_skybox.h"
#include "../../../helper/texture.h"
#include <glm/ext/matrix_transform.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneSkybox::SceneSkybox() : skybox_(100.0f){}

SceneSkybox::~SceneSkybox()
{
	defualt_cleanup();

	if (tex_ != 0) {
		glDeleteTextures(1, &tex_);
		tex_ = 0;
	}
}

void SceneSkybox::init()
{
	tex_ = Texture::loadCubeMap("media/texture/yokohama/yokohama", ".jpg");

	modelMatrix_ = mat4(1.0f);
	modelMatrix_ = glm::translate(modelMatrix_, vec3(0.0f, 11.0f, 0.0f));
}

void SceneSkybox::update(float t)
{
}

void SceneSkybox::render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_);

	apply_model_matrix(prog, view, projection);

	skybox_.render();
}

void SceneSkybox::renderDepth(GLSLProgram& prog)
{
}
