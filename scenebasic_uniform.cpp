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
#include "src/objects/decos/spotlight_point.h"
#include "src/ui/menu/menu.h"
#include "src/objects/floor/floor.h"
#include "src/objects/cars/showcase_car/showcase_car.h"
#include <memory>
#include <vector>
#include "src/objects/decos/particle_point.h"


using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

using std::string;
using std::cerr;

SceneBasic_Uniform::SceneBasic_Uniform()
{
	model = mat4(1.0f);
	view = CamControls::getViewMatrix();
	projection = mat4(1.0f);
}

void SceneBasic_Uniform::initScene()
{
	// shader setup -------
	compile_shaders();

	// ui setup -------
	init_ui();

	// skybox setup -------
	skyboxProg_.use();
	complexProg_.setUniform("SkyBoxTex", 0);
	skybox_.init();


	// common object setup
	auto spotlight = std::make_shared<SpotlightPoint>();
	auto particlePoint = std::make_shared<ParticlePoint>();

	// complex setup ---------------

	complexObjs_.push_back(std::make_shared<ShowcaseCar>());
	complexObjs_.push_back(std::make_shared<Floor>());
	complexObjs_.push_back(spotlight);
	complexObjs_.push_back(particlePoint);

	for (auto& obj : complexObjs_)
		obj->init();

	// uploader setup --------------

	uploaderObjs_.push_back(spotlight);

	// particle setup ---------

	particleObjs_.push_back(particlePoint);

	// light setup -------
	lightObjs_.push_back(spotlight);
}


void SceneBasic_Uniform::compile_shaders()
{
	try {
		ShaderInclude::process("shader/complex/complex.frag");
		complexProg_.compileShader("shader/complex/complex.vert");
		complexProg_.compileShader("shader/out/complex.frag");
		complexProg_.link();
		complexProg_.use();

		skyboxProg_.compileShader("shader/basic_uniform.vert");
		skyboxProg_.compileShader("shader/skybox/skybox.frag");
		skyboxProg_.link();
		skyboxProg_.use();

		particleProg_.compileShader("shader/particles/particle.frag");
		particleProg_.compileShader("shader/particles/particle.vert");
		particleProg_.link();
		particleProg_.use();
	}
	catch (GLSLProgramException& e) {
		cerr << e.what() << '\n';
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::init_ui()
{
	uiElements_.emplace_back(new Menu());
	ImGuiCore::Init(glfwGetCurrentContext());
	ImGuiCore::BeginFrame();
	for (auto& ui : uiElements_)
		ui->init();
	ImGuiCore::EndFrame();
}

void SceneBasic_Uniform::update(float t)
{
	for (auto& obj : complexObjs_)
		obj->update(t);

	for (auto& upl : uploaderObjs_)
		upl->upload(globalSettings);
	
	globalSettings.updateGPU();

	for (auto& ui : uiElements_)
		ui->update();

	// update view
	view = CamControls::getViewMatrix();
	projection = glm::perspective(glm::radians(80.0f), static_cast<float>(width) / height, 0.3f, 200.0f);

	Window::updateKeyState();
}

void SceneBasic_Uniform::render()
{
	post_processor::get_instance().begin_scene_capture();

	draw_scene();

	post_processor::get_instance().apply_post_render();

	draw_ui();
}

void SceneBasic_Uniform::draw_scene() {

	//render lights
	for (auto& obj : lightObjs_)
		obj->renderLight(view, projection);

	// render skybox
	skyboxProg_.use();
	skybox_.render(view, projection, skyboxProg_);
	
	// render complex
	complexProg_.use();
	for (auto& obj : complexObjs_)
		obj->render(view, projection, complexProg_);

	// render particle
	particleProg_.use();
	for (auto& obj : particleObjs_)
		obj->renderParticles(view, projection, particleProg_);
}

void SceneBasic_Uniform::draw_ui()
{
	ImGuiCore::BeginFrame();
	for (auto& ui : uiElements_)
		ui->render();
	ImGuiCore::EndFrame();
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


