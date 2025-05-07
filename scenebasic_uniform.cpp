#include "scenebasic_uniform.h"

#include <cstdlib>
#include <string>
#include <array>
#include <iostream>
#include <GLFW/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"
#include "camControls.h"
#include "src/rendering/post_processing/post_process_manager.h"
#include "src/shader_management/shader_include/shader_include.h"
#include "src/ui/imgui_wrapper/imgui_core.h"

#include "src/window/window.h"
#include "helper/texture.h"


using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

using std::string;
using std::cerr;

SceneBasic_Uniform::SceneBasic_Uniform():
		t_prev_(0), angle_(0)
{
	model = mat4(1.0f);
	view = CamControls::getViewMatrix();
	projection = mat4(1.0f);
}

void SceneBasic_Uniform::initScene()
{
	compile_shaders();
	init_ui();

	// skybox setup -------
	skyboxProg_.use();
	prog2_.setUniform("SkyBoxTex", 0);
	skybox_.init();

	// spawner setup ---------
	spawnerProg_.use();

	particleTex_ = Texture::loadTexture("media/texture/particles/spark.png");

	spawner_.init(spawnerProg_,
		particleTex_,
		500,
		2,
		vec3(0,0,0),
		glm::vec3(0.f, 1.0f, 0.f),
		1.f);


	// object setup ---------------

	objects_.emplace_back(new ShowcaseCar());
	objects_.emplace_back(new Floor());

	for (auto& obj : objects_)
		obj->init();
}


void SceneBasic_Uniform::compile_shaders()
{
	try {
		prog_.compileShader("shader/basic_uniform.vert");
		prog_.compileShader("shader/basic_uniform.frag");
		prog_.link();
		prog_.use();

		ShaderInclude::process("shader/complex/complex.frag");
		prog2_.compileShader("shader/complex/complex.vert");
		prog2_.compileShader("shader/out/complex.frag");
		prog2_.link();
		prog2_.use();

		skyboxProg_.compileShader("shader/basic_uniform.vert");
		skyboxProg_.compileShader("shader/skybox/skybox.frag");
		skyboxProg_.link();
		skyboxProg_.use();

		spawnerProg_.compileShader("shader/particles/particle.frag");
		spawnerProg_.compileShader("shader/particles/particle.vert");
		spawnerProg_.link();
		spawnerProg_.use();
	}
	catch (GLSLProgramException& e) {
		cerr << e.what() << '\n';
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::init_ui()
{
	ImGuiCore::Init(glfwGetCurrentContext());
	ImGuiCore::BeginFrame();
	menu.init();
	ImGuiCore::EndFrame();
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
	
	for (auto& obj : objects_)
		obj->update(t);


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


	if (Window::isKeyPressedOnce(GLFW_KEY_W)) {
		spawner_.fire(t);
	}

	// advance particle time
	spawner_.update(t);

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

	skyboxProg_.use();
	skybox_.render(view, projection, skyboxProg_);
	
	prog2_.use();
	for (auto& obj : objects_)
		obj->render(view, projection, prog2_);

	// render spawner
	spawnerProg_.use();
	spawner_.render(view, projection);
}

void SceneBasic_Uniform::resize(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	projection = glm::perspective(glm::radians(70.0f), static_cast<float>(w) / h, 0.3f, 100.0f);

	post_processor::get_instance().resize(w, h);
}

void SceneBasic_Uniform::set_matrices(GLSLProgram& prog) {
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);
}


