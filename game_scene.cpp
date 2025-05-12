#include "game_scene.h"
#include "src/rendering/post_processing/post_process_manager.h"
#include "src/shader_management/shader_include/shader_include.h"
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include "src/objects/decos/spotlight_point.h"
#include "src/objects/floor/floor.h"
#include "src/window/window.h"
#include "src/objects/barrel/barrel.h"
#include "src/objects/cars/game_car/game_car.h"

GameScene::GameScene()
{
	view = camera.GetViewMatrix();
	projection = glm::perspective(glm::radians(80.0f),
		float(width) / height,
		0.3f, 200.0f);
}

GameScene::~GameScene()
{
}

void GameScene::initScene()
{
	compile_shaders();

	car_ = std::make_shared<GameCar>();
	auto spotlight1 = std::make_shared<SpotlightPoint>();
	auto spotlight2 = std::make_shared<SpotlightPoint>();

	spotlight1->set_rotate_pos(glm::vec3(0,0,-20));
	spotlight2->set_rotate_pos(glm::vec3(0, 0, 20));

	auto barrel1 = std::make_shared<Barrel>();
	auto barrel2 = std::make_shared<Barrel>();

	barrel1->set_position(glm::vec3(0, 0, -20));
	barrel2->set_position(glm::vec3(0, 0, 20));

	// complex setup ---------------

	complexObjs_.push_back(spotlight1);
	complexObjs_.push_back(spotlight2);
	complexObjs_.push_back(barrel1);
	complexObjs_.push_back(barrel2);
	complexObjs_.push_back(std::make_shared<Floor>());
	complexObjs_.push_back(car_);

	for (auto& obj : complexObjs_)
		obj->init();

	// collision setup -------------

	collisionObjs_.push_back(barrel1);
	collisionObjs_.push_back(barrel2);
	collisionObjs_.push_back(car_);

	collisionManager.addObject(car_);
	collisionManager.addObject(barrel1);
	collisionManager.addObject(barrel2);

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
	camera.Update(t, car_->get_position(), car_->get_rotation());

	for (auto& obj : complexObjs_)
		obj->update(t);

	globalSettings.clearSpotlights();

	for (auto& upl : uploaderObjs_)
		upl->upload(globalSettings);

	globalSettings.updateGPU();

	for (auto& ui : uiElements_)
		ui->update();

	// update view
	view = camera.GetViewMatrix();
	projection = glm::perspective(glm::radians(80.0f), static_cast<float>(width) / height, 0.3f, 200.0f);

	collisionManager.detectAndNotify();
	collisionManager.detectOnceAndNotify();

	Input::updateKeyState();
}

void GameScene::render()
{
	draw_shadow_maps();

	post_processor::get_instance().begin_scene_capture();

	draw_scene();

	post_processor::get_instance().apply_post_render();
}

void GameScene::draw_scene()
{
	//render lights
	for (auto& obj : lightObjs_)
		obj->render_light(view, projection);

	// render skybox
	//skyboxProg_.use();
	//skybox_.render(view, projection, skyboxProg_);

	// render complex
	complexProg_.use();

	int N = static_cast<int>(lightObjs_.size());
	for (int i = 0; i < N; i++) {
		auto& light = lightObjs_[i];

		light->calculate_light_space_matrix();
		glActiveTexture(GL_TEXTURE0 + SceneObject::LIGHT_UNIT + i);
		glBindTexture(GL_TEXTURE_2D, light->get_shadow_tex());

		std::string shadowsName = std::string("ShadowMaps[") + std::to_string(i) + "]";
		std::string matrixName = std::string("ShadowMatrices[") + std::to_string(i) + "]";

		complexProg_.setUniform("numShadows", i + 1);
		complexProg_.setUniform(shadowsName.c_str(), SceneObject::LIGHT_UNIT + i);
		complexProg_.setUniform(matrixName.c_str(), LightObject::SHADOW_BIAS * light->get_light_space_matrix());
	}

	for (auto& obj : complexObjs_)
		obj->render(view, projection, complexProg_);

	// render particle
	particleProg_.use();
	for (auto& obj : particleObjs_)
		obj->renderParticles(view, projection, particleProg_);

	// render clouds
	//cloudProg_.use();
	//clouds.render(view, projection, cloudProg_);
	
	glBindVertexArray(collisionDebugRenderer.getVAO());

	collisionProg_.use();
	for (auto& obj : collisionObjs_)
		obj->draw_collision_bounds(collisionProg_, projection, view);

	glBindVertexArray(0);

}

void GameScene::draw_shadow_maps()
{
	depthProg_.use();

	for (auto& light : lightObjs_) {
		light->calculate_light_space_matrix();

		glBindFramebuffer(GL_FRAMEBUFFER, light->get_shadow_fbo());
		glViewport(0, 0, light->get_shadow_res(), light->get_shadow_res());
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(2.5f, 10.0f);
		glDepthMask(GL_TRUE);


		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glClear(GL_DEPTH_BUFFER_BIT);

		depthProg_.setUniform("lightSpaceMatrix", light->get_light_space_matrix());

		for (auto& obj : complexObjs_) {
			obj->renderDepth(depthProg_);
		}

		glFlush();
	}

	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
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

		collisionProg_.compileShader("shader/collision/collision_box.vert");
		collisionProg_.compileShader("shader/collision/collision_box.frag");
		collisionProg_.link();

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
	projection = glm::perspective(glm::radians(90.0f), static_cast<float>(w) / h, 0.3f, 100.0f);

	post_processor::get_instance().resize(w, h);
}