#pragma once

#include "../scene_object.h"
#include "../uploader_object.h"

#include "../light_object.h"

#include "../../global_settings/lights/spotlight/spotlight.h"

class SpotlightPoint : public SceneObject, public UploaderObject, public LightObject
{
public:
	SpotlightPoint();
	~SpotlightPoint() = default;

	void init() override;
	void update(float t) override;
	void render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) override;
	void renderDepth(GLSLProgram& depthProg) override;

	void calculate_light_space_matrix() override;
	void render_light(const glm::mat4& view, const glm::mat4& projectiong) override;
	void upload(GlobalSettingsUBO& globalUbo) override;

private:
	spotlight spotlight_;
	float t_prev_;
	float angle_;
	float radius_;
	float rotationSpeed_;
	glm::vec3 lightDir_;
	glm::vec4 lightPos_;
	glm::vec3 rotatePoint_;
};