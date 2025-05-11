#pragma once

#include "../../scene_object.h"
#include <memory>
#include "../../../../helper/objmesh.h"
class GameCar : public SceneObject 
{
public:
	GameCar();
	~GameCar();

	void init() override;
	void update(float t) override;
	void render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) override;
	void renderDepth(GLSLProgram& depthProg) override;
private:
	std::unique_ptr<ObjMesh> car_;

};