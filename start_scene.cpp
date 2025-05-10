#include "start_scene.h"
#include "src/ui/imgui_wrapper/imgui_core.h"
#include "src/ui/menu/start_menu.h"
#include <glm/ext/matrix_clip_space.hpp>

void StartScene::initScene()
{
	uiElements_.emplace_back(new StartMenu());
	//ImGuiCore::Init(glfwGetCurrentContext());
	ImGuiCore::BeginFrame();
	for (auto& ui : uiElements_)
		ui->init();
	ImGuiCore::EndFrame();
}

void StartScene::update(float t)
{
}

void StartScene::render()
{
	ImGuiCore::BeginFrame();
	for (auto& ui : uiElements_)
		ui->render();

	ImGuiCore::EndFrame();
}

void StartScene::resize(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	projection = glm::perspective(glm::radians(70.0f), static_cast<float>(w) / h, 0.3f, 100.0f);

}
