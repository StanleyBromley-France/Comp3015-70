#include "scenebasic_uniform.h"

#include <cstdlib>
#include <string>
#include <array>
#include <iostream>
#include <GLFW/glfw3.h>

#include "glm/gtc/matrix_transform.hpp"
#include "helper/glutils.h"
#include "helper/texture.h"
#include "camControls.h"
#include "src/rendering/post_processing/post_process_manager.h"
#include "src/shader_management/shader_include/shader_include.h"
#include "src/ui/imgui_wrapper/imgui_core.h"

#include "src/window/window.h"


using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

using std::string;
using std::cerr;

SceneBasic_Uniform::SceneBasic_Uniform(): platform_(500.0f, 500.0f, 1, 1), skybox_(100.0f),
		t_prev_(0), angle_(0)
{
}

void SceneBasic_Uniform::initScene()
{
	compile();
	glEnable(GL_DEPTH_TEST);
	model = mat4(1.0f);
	view = CamControls::getViewMatrix();
	projection = mat4(1.0f);

	// light settings

	spotlight_.upload(globalSettings);
	//globalSettings.setLightingMode(1);
	globalSettings.updateGPU();

	// texture

	GLuint cubeTex = Texture::loadCubeMap("media/texture/yokohama/yokohama", ".jpg");

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

	//prog_.setUniform("IsToonLighting", toon_shading_);

	showcase_car_.init(prog2_);

	ImGuiCore::Init(glfwGetCurrentContext());

	ImGuiCore::BeginFrame();

	menu.init();

	ImGuiCore::EndFrame();

	post_processor::get_instance().resize(2560, 1440);
}


void SceneBasic_Uniform::compile()
{
	try {
		prog_.compileShader("shader/basic_uniform.vert");
		prog_.compileShader("shader/basic_uniform.frag");
		prog_.link();
		prog_.use();

		ShaderInclude::process("shader/car/car.frag");
		prog2_.compileShader("shader/car/car.vert");
		prog2_.compileShader("shader/out/car.frag");
		prog2_.link();
		prog2_.use();
	}
	catch (GLSLProgramException& e) {
		cerr << e.what() << '\n';
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update(float t)
{
	// update light pos
	float rotationSpeed = .25f;

	float deltaT = t - t_prev_;
	if (t_prev_ == 0.0f) deltaT = 0.0f;
	t_prev_ = t;

	angle_ += rotationSpeed * deltaT;
	if (angle_ > glm::two_pi<float>()) angle_ -= glm::two_pi<float>();
	
	// toon shading toggle
	static bool tKeyPressed = false;
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_T) == GLFW_PRESS) {
		if (!tKeyPressed) {
			toon_shading_ = !toon_shading_; // Toggle toonShading
			tKeyPressed = true; // Mark key as pressed
			prog_.setUniform("IsToonLighting", toon_shading_);

		}
	}
	else {
		tKeyPressed = false;
	}
	
	// normal map toggle
	static bool nKeyPressed = false;
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_N) == GLFW_PRESS) {
		if (!nKeyPressed) {
			nKeyPressed = true;
			normal_mode_ = !normal_mode_;
			prog_.setUniform("DoNormalMap", normal_mode_);
		}
	}
	else {
		nKeyPressed = false;
	}

	showcase_car_.update(t);

	// Light position: Rotate diagonally around the origin

	auto radius = 35.0f; // Distance from the origin
	auto light_pos = vec4(radius * cos(angle_), radius, radius * sin(angle_), 1.0f); // Diagonal rotation

	// Transform light position to view space
	spotlight_.set_position(vec3(view * light_pos));

	// Calculate light direction: Point from light position to origin (0, 0, 0)
	const auto light_dir = normalize(vec3(0.0f) - vec3(light_pos)); // Direction toward origin
	const auto normal_matrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
	spotlight_.set_direction(normal_matrix * light_dir);

	spotlight_.upload(globalSettings);
	globalSettings.updateGPU();

	menu.update();

	Window::updateKeyState();
}

void SceneBasic_Uniform::render()
{
	post_processor::get_instance().begin_scene_capture();
	
	prog_.use();
	
	// update view
	view = CamControls::getViewMatrix();
	projection = glm::perspective(glm::radians(80.0f), static_cast<float>(width) / height, 0.3f, 200.0f);

	draw_scene();

	post_processor::get_instance().apply_post_render();

	ImGuiCore::BeginFrame();
	menu.render();
	ImGuiCore::EndFrame();

}

void SceneBasic_Uniform::draw_scene() {
	// render skybox

	prog_.setUniform("IsSkyBox", true);
	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, 11.0f, 0.0f));
	set_matrices();
	skybox_.render();


	// render platform

	prog_.setUniform("Material.Kd", vec3(0.7f, 0.7f, 0.7f));
	prog_.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
	prog_.setUniform("Material.Ka", vec3(0.2f, 0.2f, 0.2f));
	prog_.setUniform("Material.Shininess", 25.f);
	prog_.setUniform("IsTextured", false);
	prog_.setUniform("IsSkyBox", false);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f));
	set_matrices();
	platform_.render();
	

	showcase_car_.render(view, projection);
}

void SceneBasic_Uniform::resize(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	projection = glm::perspective(glm::radians(70.0f), static_cast<float>(w) / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::set_matrices() {
	mat4 mv = view * model;
	prog_.setUniform("ModelViewMatrix", mv);
	prog_.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	prog_.setUniform("MVP", projection * mv);
}


