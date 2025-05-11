#pragma once

#include "../scene_object.h"

class Barrel : public SceneObject 
{
public:
	void init();
	void update(float t);
	void render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog);
	void renderDepth(GLSLProgram& depthProg);
private:
	std::unique_ptr<ObjMesh> barrel_;

};