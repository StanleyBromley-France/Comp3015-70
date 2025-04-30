#include "helper/scene.h"
#include "helper/scenerunner.h"
#include "scenebasic_uniform.h"
#include "camControls.h"

#include "src/ui/imgui_wrapper/imgui_core.h"

#include "src/window/window.h"

int main(int argc, char* argv[])
{
	SceneRunner runner("Shader_Basics", 2560, 1440);

	std::unique_ptr<Scene> scene;

	scene = std::unique_ptr<Scene>(new SceneBasic_Uniform());

	//ImGuiCore::Init(glfwGetCurrentContext());

	CamControls::initialise(glfwGetCurrentContext());


	return runner.run(*scene);
}