#pragma once

#include "../scene_object.h"
#include "../../game_management/checkpoint.h"
#include "../uploader_object.h"

#include "../light_object.h"
#include "../../collision/collision_object/collision_object.h"

class GameCheckpoint : public SceneObject, public Checkpoint, public CollisionObject, public UploaderObject, public LightObject
{
public:
	GameCheckpoint();
	void init() override;
	void update(float t) override;
	void render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) override;
	void renderDepth(GLSLProgram& depthProg) override;

	void on_collision(CollisionObject& other) override;
	void on_collision_once(CollisionObject& other) override;

	void upload(GlobalSettingsUBO& globalUbo) override;

	void calculate_light_space_matrix() override;
	void render_light(const glm::mat4& view, const glm::mat4& projection) override;

	void set_rotate_pos(glm::vec3 rotPos);

private:
	// spotlight stuff
	spotlight spotlight_;
	float t_prev_;
	float angle_;
	float radius_;
	float rotationSpeed_;
	glm::vec3 lightDir_;
	glm::vec4 lightPos_;
	glm::vec3 rotatePoint_;

};