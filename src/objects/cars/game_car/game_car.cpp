#include "game_car.h"
#include "../../../save_management/save_data_manager.h"
#include "../car_colours.h"
#include <glm/ext/matrix_transform.hpp>


using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

GameCar::GameCar()
{
}

GameCar::~GameCar()
{
}

void GameCar::init()
{
	car_ = ObjMesh::load("media/model/car.obj", true);

	auto& settings = SaveDataManager::Instance().Data();
	
	albedoTextures_.resize(2);
	albedoTextures_[0] = CarColours::load(settings.carColour1);
	albedoTextures_[1] = CarColours::load(settings.carColour2);

	normalTexture_ = Texture::loadTexture("media/texture/normal.png");

	modelMatrix_ = mat4(1.0f);
	modelMatrix_ = glm::scale(modelMatrix_, vec3(5.f));
	modelMatrix_ = glm::translate(modelMatrix_, vec3(0.0f, 0.8f, 0.0f));

	// material setup
	mat.Kd = vec3(0.7f, 0.7f, 0.7f);
	mat.Ks = vec3(0.9f, 0.9f, 0.9f);
	mat.Ka = vec3(0.2f, 0.2f, 0.2f);
	mat.Shininess = 25.f;
}

void GameCar::update(float t)
{
}

void GameCar::render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog)
{
	apply_textures(prog);

	applyMat(prog);

	apply_model_matrix(prog, view, projection);

	prog.setUniform("useSpecular", true);

	car_->render();
}

void GameCar::renderDepth(GLSLProgram& depthProg)
{
	depthProg.setUniform("model", modelMatrix_);
	car_->render();
}
