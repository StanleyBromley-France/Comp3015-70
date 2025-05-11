#include "game_scene.h"
#include "src/rendering/post_processing/post_process_manager.h"
#include "src/shader_management/shader_include/shader_include.h"
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include "src/objects/decos/spotlight_point.h"
#include "src/objects/floor/floor.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::initScene()
{
	compile_shaders();

	auto spotlight1 = std::make_shared<SpotlightPoint>();
	auto spotlight2 = std::make_shared<SpotlightPoint>();

	spotlight1->set_rotate_pos(glm::vec3(0,0,1));
	spotlight1->set_rotate_pos(glm::vec3(0, 0, 2));

	// complex setup ---------------

	complexObjs_.push_back(spotlight1);
	complexObjs_.push_back(spotlight2);
	complexObjs_.push_back(std::make_shared<Floor>());


	for (auto& obj : complexObjs_)
		obj->init();

	// uploader setup --------------

	uploaderObjs_.push_back(spotlight1);
	uploaderObjs_.push_back(spotlight2);


	// particle setup ---------


	// light setup -------
	lightObjs_.push_back(spotlight1);
	lightObjs_.push_back(spotlight2);


	for (auto& obj : lightObjs_)
		obj->initShadowMap();
}

void GameScene::update(float t)
{
	for (auto& obj : complexObjs_)
		obj->update(t);

	for (auto& upl : uploaderObjs_)
		upl->upload(globalSettings);

	globalSettings.updateGPU();

	for (auto& ui : uiElements_)
		ui->update();
}

void GameScene::render()
{
	post_processor::get_instance().begin_scene_capture();

	draw_scene();

	post_processor::get_instance().apply_post_render();
}

void GameScene::draw_scene()
{
	
}

void GameScene::compile_shaders()
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

		depthProg_.compileShader("shader/depth/depth.vert");
		depthProg_.compileShader("shader/depth/depth.frag");
		depthProg_.link();

		cloudProg_.compileShader("shader/sky/sky.vert");
		cloudProg_.compileShader("shader/sky/sky.frag");
		cloudProg_.link();

	}
	catch (GLSLProgramException& e) {
		std::cerr << e.what() << '\n';
		exit(EXIT_FAILURE);
	}
}


void GameScene::resize(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	projection = glm::perspective(glm::radians(70.0f), static_cast<float>(w) / h, 0.3f, 100.0f);

	post_processor::get_instance().resize(w, h);
}