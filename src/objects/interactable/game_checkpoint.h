#pragma once

#include "../scene_object.h"
#include "../../game_management/checkpoint.h"

#include "../../collision/collision_object/collision_object.h"

class GameCheckpoint : public SceneObject, public Checkpoint, public CollisionObject
{
public:
	void init() override;
	void update(float t) override;
	void render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) override;
	void renderDepth(GLSLProgram& depthProg) override;

	void on_collision(CollisionObject& other) override;
	void on_collision_once(CollisionObject& other) override;
};