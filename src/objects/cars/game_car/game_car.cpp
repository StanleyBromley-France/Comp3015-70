#include "game_car.h"
#include "../../../save_management/save_data_manager.h"
#include "../car_colours.h"
#include <glm/ext/matrix_transform.hpp>
#include "../../../window/window.h"
#include <iostream>


using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

GameCar::GameCar()
{
}

GameCar::~GameCar()
{
}

void GameCar::init()
{
	car_ = ObjMesh::load("media/model/car.obj", true);

	auto& settings = SaveDataManager::Instance().Data();
	
	albedoTextures_.resize(2);
	albedoTextures_[0] = CarColours::load(settings.carColour1);
	albedoTextures_[1] = CarColours::load(settings.carColour2);

	normalTexture_ = Texture::loadTexture("media/texture/normal.png");

	modelMatrix_ = mat4(1.0f);
	modelMatrix_ = glm::scale(modelMatrix_, vec3(5.f));
	modelMatrix_ = glm::translate(modelMatrix_, vec3(0.0f, 0.8f, 0.0f));

	// material setup
	mat.Kd = vec3(0.7f, 0.7f, 0.7f);
	mat.Ks = vec3(0.9f, 0.9f, 0.9f);
	mat.Ka = vec3(0.2f, 0.2f, 0.2f);
	mat.Shininess = 25.f;

	position_ = vec3(0.f, 0.8f, 0.f);
	yawAngle_ = 0.f;
	velocity_ = 0.f;
	lastFrameTime_ = glfwGetTime();

    update_collider_position(glm::vec2(position_.x, position_.z));
    set_collider_size(glm::vec2(10.f, 25.f));
}

void GameCar::update(float t)
{
	float dt = t - lastFrameTime_;
	lastFrameTime_ = t;
	Input::updateKeyState();

    bool fwd = Input::isKeyPressed(GLFW_KEY_W);
    bool rev = Input::isKeyPressed(GLFW_KEY_S);
    bool left = Input::isKeyPressed(GLFW_KEY_A);
    bool right = Input::isKeyPressed(GLFW_KEY_D);

    // handles velosity based on inputs
    if (fwd)
        velocity_ += accel_ * dt;
    else if (rev)
        velocity_ -= brakeAccel_ * dt;
    else if (velocity_ != 0.f)
        velocity_ -= drag_ * dt * glm::sign(velocity_);

    // clamps speed to max values
    velocity_ = glm::clamp(velocity_, maxReverseSpeed_, maxForwardSpeed_);

    // turns, scaled by speed
    float speedFrac = glm::clamp(glm::abs(velocity_) / maxForwardSpeed_, 0.f, 1.f);
    float appliedTurn = turnSpeed_ * speedFrac * dt;
    if (left) yawAngle_ += appliedTurn;
    if (right) yawAngle_ -= appliedTurn;

    // computes forward right vectors
    glm::vec3 forwardDir = glm::vec3(sin(yawAngle_), 0.f, cos(yawAngle_));
    glm::vec3 rightDir = glm::cross(forwardDir, vec3(0, 1, 0));

    // updates position + slip
    position_ += forwardDir * velocity_ * dt;
    position_ += rightDir * driftScale_ * appliedTurn * glm::sign(velocity_);

    // re-builds  model matrix
    modelMatrix_ = glm::translate(mat4(1.f), position_);
    modelMatrix_ = glm::rotate(modelMatrix_, yawAngle_, vec3(0, 1, 0));

    update_collider_angle(yawAngle_);

    // body roll, tilts into the turn
    float steerInput = (right ? 1.f : (left ? -1.f : 0.f));
    float rollAngle = glm::radians(5.f) * speedFrac * steerInput;
    modelMatrix_ = glm::rotate(modelMatrix_, rollAngle, vec3(0, 0, 1));

    // Uniform scale
    modelMatrix_ = glm::scale(modelMatrix_, vec3(5.f));
    modelMatrix_ = glm::translate(modelMatrix_, vec3(0.0f, 0.6f, 0.0f));

    update_collider_position(glm::vec2(position_.x, position_.z));
}

void GameCar::render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog)
{
	apply_textures(prog);

	applyMat(prog);

	apply_model_matrix(prog, view, projection);

	prog.setUniform("useSpecular", true);

	car_->render();
}

void GameCar::renderDepth(GLSLProgram& depthProg)
{
	depthProg.setUniform("model", modelMatrix_);
	car_->render();
}

void GameCar::on_collision(CollisionObject& other)
{
   // std::cout << "hit barrel!" << "\n";
}

void GameCar::on_collision_once(CollisionObject& other)
{
    std::cout << "hit barrel!" << "\n";
}
