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
	float lastFrameTime_;

    float yawAngle_{ 0.f };
    float velocity_{ 0.f };

    // tuning parameters
    const float maxForwardSpeed_ = 30.f;
    const float maxReverseSpeed_ = -10.f;
    const float accel_ = 25.f;
    const float brakeAccel_ = 50.f; 
    const float drag_ = 5.f; // natural deceleration
    const float turnSpeed_ = 2.f;
    const float driftScale_ = 0.5f; // drift multiplier
};