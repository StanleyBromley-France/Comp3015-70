#include "particle_point.h"
#include "../../../helper/texture.h"
#include "../../window/window.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;



ParticlePoint::ParticlePoint() : particleCount_(500), particleDir_(vec3(0.f, 1.0f, 0.f)), particlePos_(vec3(0)), particleSize_(1.f), particleLifetime_(2.f)
{
}

ParticlePoint::~ParticlePoint()
{
	defualt_cleanup();  // Call SceneObject's cleanup if it exists

	if (particleTex_ != 0) {
		glDeleteTextures(1, &particleTex_);
		particleTex_ = 0;
	}

}

void ParticlePoint::init()
{
	particleTex_ = Texture::loadTexture("media/texture/particles/spark.png");

	spawner_.init(particleTex_, particleCount_, particleLifetime_, particlePos_, particleDir_, particleSize_);
}

void ParticlePoint::update(float t)
{
	if (Input::isKeyPressedOnce(GLFW_KEY_W)) {
		spawner_.fire(t);
	}

	// advances particle time
	spawner_.update(t);
}

void ParticlePoint::render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog)
{
}

void ParticlePoint::renderDepth(GLSLProgram& depthProg) {}

void ParticlePoint::renderParticles(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog)
{
	spawner_.render(view, projection, prog);
}
