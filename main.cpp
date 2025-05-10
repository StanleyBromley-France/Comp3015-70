#include "helper/scene.h"
#include "helper/scenerunner.h"
#include "scenebasic_uniform.h"

#include "start_scene.h"

#include "camControls.h"
#include "src/window/window.h"
#include "src/save_management/save_data_manager.h"


int main(int argc, char* argv[])
{
	SceneRunner runner("Car Game", 1280, 720);

	std::unique_ptr<Scene> scene;

	scene = std::unique_ptr<Scene>(new StartScene());

	SaveDataManager::Instance().Load();

	Input::createInstance();
	CamControls::initialise(glfwGetCurrentContext());

	ImGuiCore::Init(glfwGetCurrentContext());

	return runner.run(std::move(scene));
}