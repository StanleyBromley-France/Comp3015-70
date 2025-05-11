#include "spotlight_point.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

using glm::vec3;
using glm::vec4;
using glm::mat3;

SpotlightPoint::SpotlightPoint() : radius_(35.f), rotationSpeed_(.25f), rotatePoint_(vec3(0)), angle_(0), lightPos_(0), t_prev_(0) {}

SpotlightPoint::~SpotlightPoint()
{
	defualt_cleanup();
	default_cleanup_light();

}

void SpotlightPoint::init() 
{
}

void SpotlightPoint::update(float t)
{
	// update light pos

	float deltaT = t - t_prev_;
	if (t_prev_ == 0.0f) deltaT = 0.0f;
	t_prev_ = t;

	angle_ += rotationSpeed_ * deltaT;
	if (angle_ > glm::two_pi<float>()) angle_ -= glm::two_pi<float>();

	// light position: Rotate diagonally around the origin
	lightPos_ = vec4(radius_ * cos(angle_), radius_, radius_ * sin(angle_), 1.0f); // diagonal rotation
	lightDir_ = rotatePoint_ - vec3(lightPos_); // Direction toward origin

}

void SpotlightPoint::render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) {}

void SpotlightPoint::renderDepth(GLSLProgram& depthProg) {}

void SpotlightPoint::calculate_light_space_matrix()
{ 

	auto& data = spotlight_.data_;
	float fov = data.Cutoff * 2;

	glm::mat4 V = glm::lookAt(vec3(lightPos_), vec3(lightPos_) + lightDir_, glm::vec3(0, 1, 0));
	glm::mat4 P = glm::perspective(fov, 1.0f, 0.3f, 100.0f );
	lightSpaceMatrix_ = P * V;
}

void SpotlightPoint::render_light(const glm::mat4& view, const glm::mat4& projectiong)
{
	// calculates light direction: Point from light position to origin (0, 0, 0)
	const auto normal_matrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));

	spotlight_.set_position(vec3(view * lightPos_));
	spotlight_.set_direction(normal_matrix * lightDir_);
}

void SpotlightPoint::upload(GlobalSettingsUBO& globalUbo)
{
	globalUbo.addSpotlight(spotlight_.data_);
}

void SpotlightPoint::set_rotate_pos(glm::vec3 rotPos)
{
	rotatePoint_ = rotPos;
}
