#pragma once
#include "../scene_object.h"
#include "../particle_object.h"
#include "../../global_settings/lights/particle_spawner/particle_spawner.h"


class ParticlePoint : public SceneObject, public ParticleObject
{
public:
	ParticlePoint();
	~ParticlePoint() = default;

	void init() override;
	void update(float t) override;
	void render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) override;
	void renderParticles(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) override;
private:
	GLuint particleTex_;
	ParticleSpawner spawner_;
	float particleLifetime_;
	int particleCount_;
	glm::vec3 particleDir_;
	glm::vec3 particlePos_;
	float particleSize_;
};