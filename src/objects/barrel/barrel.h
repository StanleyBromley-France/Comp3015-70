#pragma once

#include "../scene_object.h"
#include "../../collision/collision_object/collision_object.h"
class Barrel : public SceneObject, public CollisionObject
{
public:
	void init();
	void update(float t);
	void render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog);
	void renderDepth(GLSLProgram& depthProg);

	void on_collision(CollisionObject& other) override;
	void on_collision_once(CollisionObject& other) override;
private:
	std::unique_ptr<ObjMesh> barrel_;

};