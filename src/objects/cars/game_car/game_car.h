#pragma once

#include "../../scene_object.h"
#include <memory>
#include "../../../../helper/objmesh.h"
#include "../../../collision/collision_object/collision_object.h"
class GameCar : public SceneObject, public CollisionObject 
{
public:
	GameCar();
	~GameCar();

	void init() override;
	void update(float t) override;
	void render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) override;
	void renderDepth(GLSLProgram& depthProg) override;

    void on_collision(CollisionObject& other) override;
    void on_collision_once(CollisionObject& other) override;
private:

    void rebuild_model_matrix();

    glm::vec3 previousPosition_;
    float previousYawAngle_;

    float pushbackFactor_ = 0.3f;
    float velocityDamping_ = 0.4f;

	std::unique_ptr<ObjMesh> car_;
	float lastFrameTime_;

    float yawAngle_{ 0.f };
    float velocity_{ 0.f };
    glm::vec3 forwardDir_;

    // tuning parameters
    const float maxForwardSpeed_ = 30.f;
    const float maxReverseSpeed_ = -10.f;
    const float accel_ = 25.f;
    const float brakeAccel_ = 50.f; 
    const float drag_ = 5.f; // natural deceleration
    const float turnSpeed_ = 2.f;
    const float driftScale_ = 0.5f; // drift multiplier
};