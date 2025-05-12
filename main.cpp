#include "helper/scene.h"
#include "helper/scenerunner.h"
#include "scenebasic_uniform.h"

#include "start_scene.h"
#include "game_scene.h"
#include "src/window/window.h"
#include "src/save_management/save_data_manager.h"


int main(int argc, char* argv[])
{
	SceneRunner runner("Car Game", 1280, 720);

	std::unique_ptr<Scene> scene;

	ImGuiCore::Init(glfwGetCurrentContext());

	scene = std::unique_ptr<Scene>(new SceneBasic_Uniform());

	SaveDataManager::Instance().Load();

	Input::createInstance();


	return runner.run(std::move(scene));
}