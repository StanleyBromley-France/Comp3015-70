#include "game_checkpoint.h"
#include "../../game_management/game_manager.h"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat3;

GameCheckpoint::GameCheckpoint() : radius_(70.f), rotationSpeed_(.25f), rotatePoint_(vec3(0)), angle_(0), lightPos_(0), t_prev_(0)
{
}

void GameCheckpoint::init()
{
    std::cout << "creating checkpoint \n";
    update_collider_position(glm::vec2(position_.x, position_.z));
    set_collider_size(glm::vec2(15.f, 3.f));
    update_collider_angle(rotation_);
    set_is_solid(false);
}

void GameCheckpoint::update(float t)
{
	// update light pos

	float deltaT = t - t_prev_;
	if (t_prev_ == 0.0f) deltaT = 0.0f;
	t_prev_ = t;

	angle_ += rotationSpeed_ * deltaT;
	if (angle_ > glm::two_pi<float>()) angle_ -= glm::two_pi<float>();

	// light position: rotates diagonally around (0,0,0)
	vec3 localPos = vec3(
		radius_ * cos(angle_),
		radius_,
		radius_ * sin(angle_)
	);

	// translates the orbit so its center is rotatePoint_
	lightPos_ = vec4(rotatePoint_ + localPos, 1.0f);

	lightDir_ = rotatePoint_ - vec3(lightPos_); // direction toward origin
}

void GameCheckpoint::render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog)
{
}

void GameCheckpoint::renderDepth(GLSLProgram& depthProg)
{
}

void GameCheckpoint::on_collision(CollisionObject& other)
{
    GameManager::instance().arrive_at_checkpoint(checkpointId_);
}

void GameCheckpoint::on_collision_once(CollisionObject& other)
{
}

void GameCheckpoint::upload(GlobalSettingsUBO& globalUbo)
{
	if(isCheckpointActive_)
		globalUbo.addSpotlight(spotlight_.data_);
}

void GameCheckpoint::calculate_light_space_matrix()
{
	auto& data = spotlight_.data_;
	float fov = data.Cutoff * 2;

	glm::mat4 V = glm::lookAt(vec3(lightPos_), vec3(lightPos_) + lightDir_, glm::vec3(0, 1, 0));
	glm::mat4 P = glm::perspective(fov, 1.0f, 0.3f, 100.0f);
	lightSpaceMatrix_ = P * V;
}

void GameCheckpoint::render_light(const glm::mat4& view, const glm::mat4& projection)
{
	// calculates light direction: Point from light position to origin (0, 0, 0)
	const auto normal_matrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));

	spotlight_.set_position(vec3(view * lightPos_));
	spotlight_.set_direction(normal_matrix * lightDir_);
}


void GameCheckpoint::set_rotate_pos(glm::vec3 rotPos)
{
	rotatePoint_ = rotPos;
}