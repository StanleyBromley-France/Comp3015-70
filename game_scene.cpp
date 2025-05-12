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
#include "src/map_managment/map_loader/map_loader.h"
#include "src/game_management/game_manager.h"
#include "src/objects/interactable/game_checkpoint.h"
#include "src/ui/imgui_wrapper/imgui_core.h"

#include "src/ui/menu/pause_menu.h"
#include "src/save_management/save_data_manager.h"

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

	// ui setup



	ImGuiCore::BeginFrame();
	menu.init();

	ImGuiCore::EndFrame();


	// skybox setup -------
	skyboxProg_.use();
	complexProg_.setUniform("SkyBoxTex", 0);
	skybox_.init();
	clouds.init();


	GameManager::instance().reset();


	auto floor = std::make_shared<Floor>();
	complexObjs_.push_back(floor);
	floor->init();

	MapLoader loader(
		car_,
		complexObjs_, particleObjs_,
		collisionObjs_, lightObjs_,
		uploaderObjs_, checkpointObjs_,
		collisionManager
	);

	loader.load_from_file("game_map.json");

	GameManager::instance().start_race();
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

	if (!GameManager::instance().has_race_ended())
		menu.currentTime_ = GameManager::instance().get_elapsed_time();
	else {
		menu.forceOpen = true;
		auto& currentBest = SaveDataManager::Instance().Data().bestTime;
		if (menu.currentTime_ < currentBest) {
			currentBest = menu.currentTime_;
			SaveDataManager::Instance().Save();
		}
	}

	menu.update();

	// update view
	view = camera.GetViewMatrix();
	projection = glm::perspective(glm::radians(80.0f), static_cast<float>(width) / height, 0.3f, 1000.0f);

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

	ImGuiCore::BeginFrame();
	menu.render();
	ImGuiCore::EndFrame();
}

void GameScene::draw_scene()
{
	//render lights
	for (auto& obj : lightObjs_)
		obj->render_light(view, projection);

	// render skybox
	skyboxProg_.use();
	skybox_.render(view, projection, skyboxProg_);

	// render complex
	complexProg_.use();

	int actualI = 0;
	int N = static_cast<int>(lightObjs_.size());
	for (int i = 0; i < N; i++) {
		auto& light = lightObjs_[i];
		if (auto sp = std::dynamic_pointer_cast<GameCheckpoint>(light)) {
			if (!sp->is_checkpoint_active()) {
				continue;
			}
		}
		light->calculate_light_space_matrix();
		glActiveTexture(GL_TEXTURE0 + SceneObject::LIGHT_UNIT + actualI);
		glBindTexture(GL_TEXTURE_2D, light->get_shadow_tex());

		std::string shadowsName = std::string("ShadowMaps[") + std::to_string(actualI) + "]";
		std::string matrixName = std::string("ShadowMatrices[") + std::to_string(actualI) + "]";

		complexProg_.setUniform("numShadows", actualI + 1);
		complexProg_.setUniform(shadowsName.c_str(), SceneObject::LIGHT_UNIT + actualI);
		complexProg_.setUniform(matrixName.c_str(), LightObject::SHADOW_BIAS * light->get_light_space_matrix());
		actualI++;
	}

	for (auto& obj : complexObjs_)
		obj->render(view, projection, complexProg_);

	// render particle
	particleProg_.use();
	for (auto& obj : particleObjs_)
		obj->renderParticles(view, projection, particleProg_);

	// render clouds
	cloudProg_.use();
	clouds.render(view, projection, cloudProg_);
	
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

		if (auto sp = std::dynamic_pointer_cast<GameCheckpoint>(light)) {
			if (!sp->is_checkpoint_active())
				continue;
		}
		light->calculate_light_space_matrix();

		glBindFramebuffer(GL_FRAMEBUFFER, light->get_shadow_fbo());
		glViewport(0, 0, light->get_shadow_res(), light->get_shadow_res());
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(2.5f, 4.0f);
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
	glDisable(GL_DEPTH_TEST);

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
	projection = glm::perspective(glm::radians(90.0f), static_cast<float>(w) / h, 0.3f, 10000.0f);

	post_processor::get_instance().resize(w, h);
}