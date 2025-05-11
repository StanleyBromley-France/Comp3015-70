#include "showcase_car.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

#include "../../../../helper/texture.h"
#include "../../../window/window.h"
#include "../../../save_management/save_data_manager.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

using CarColours::Orange;
using CarColours::Black;
using CarColours::Blue;
using CarColours::DarkBlue;
using CarColours::DarkGrey;
using CarColours::Grey;
using CarColours::Red; 
using CarColours::COUNT;


ShowcaseCar::ShowcaseCar() : selectedColours_{ Orange, Orange }
{
}

ShowcaseCar::~ShowcaseCar()
{
    // saves user colours
    auto& settings = SaveDataManager::Instance().Data();

    settings.carColour1 = selectedColours_.first;
    settings.carColour2 = selectedColours_.second;

    SaveDataManager::Instance().Save();

    defualt_cleanup();
    cleanup();
}

void ShowcaseCar::init()
{
    // gets and stores save data for car colours
    auto& settings = SaveDataManager::Instance().Data();
    selectedColours_ = { settings.carColour1, settings.carColour2 };

    // load mesh
    car_ = ObjMesh::load("media/model/car.obj", true);
    albedoTextures_.resize(2);

    // load textures
    carColours[Orange] = CarColours::load(Orange);
    carColours[Black] = CarColours::load(Black);
    carColours[Blue] = CarColours::load(Blue);
    carColours[DarkBlue] = CarColours::load(DarkBlue);
    carColours[DarkGrey] = CarColours::load(DarkGrey);
    carColours[Grey] = CarColours::load(Grey);
    carColours[Red] = CarColours::load(Red);

    normalTexture_ = Texture::loadTexture("media/texture/normal.png");

    albedoTextures_[0] = carColours[selectedColours_.first];
    albedoTextures_[1] = carColours[selectedColours_.second];

    modelMatrix_ = mat4(1.0f);
    modelMatrix_ = glm::scale(modelMatrix_, vec3(5.f));
    modelMatrix_ = glm::translate(modelMatrix_, vec3(0.0f, 0.8f, 0.0f));

    rotationSpeed_ = 10.0f; // equivalent to 0.25f/frame at 60 FPS
    lastFrameTime_ = glfwGetTime();

    // material setup
    mat.Kd = vec3(0.7f, 0.7f, 0.7f);
    mat.Ks = vec3(0.9f, 0.9f, 0.9f);
    mat.Ka = vec3(0.2f, 0.2f, 0.2f);
    mat.Shininess = 25.f;
}

void ShowcaseCar::update(float t)
{
    set_rotation(t);
    handle_texture_selection();
}

void ShowcaseCar::set_rotation(float t)
{
    // saves last rot
    last_rot_ = rot_;

    // calculates delta time
    float delta_time = t - lastFrameTime_;
    lastFrameTime_ = t;

    // space key toggles animation
    if (Input::isKeyPressedOnce(GLFW_KEY_SPACE))
        doRotation_ = !doRotation_;

    // automatic rotation
    if (doRotation_)
        rot_ += (rotatingClockwise_ ? -rotationSpeed_ : rotationSpeed_) * delta_time;

    // manual rotation controls
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
        rot_ += rotationSpeed_ * delta_time;
        rotatingClockwise_ = false;
    }
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        rot_ -= rotationSpeed_ * delta_time;
        rotatingClockwise_ = true;
    }

    // update model matrix
    float currentRot = rot_ - lastRot_;
    modelMatrix_ = glm::rotate(modelMatrix_, glm::radians(currentRot), vec3(0.0f, 1.0f, 0.0f));
    lastRot_ = rot_;
}

void ShowcaseCar::handle_texture_selection()
{
    GLFWwindow* window = glfwGetCurrentContext();

    bool shiftHeld = (Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT));

    // Now look for a freshly pressed number key
    for (int key = GLFW_KEY_1; key <= GLFW_KEY_7; ++key)
    {
        if (Input::isKeyPressedOnce(key))
        {
            int textureIndex = key - GLFW_KEY_1;
            if (textureIndex < 0 || textureIndex >= COUNT)
                return;  // out-of-range guard

            if (shiftHeld)
                selectedColours_.first = CarColours::Index(textureIndex);
            else
                selectedColours_.second = CarColours::Index(textureIndex);

            // Swap in the new texture
            albedoTextures_[shiftHeld ? 0 : 1] = carColours[textureIndex];
            return;  // only handle one key press per frame
        }
    }
}

void ShowcaseCar::render(const mat4& view, const mat4& projection, GLSLProgram& prog)
{
    apply_textures(prog);

    applyMat(prog);

    apply_model_matrix(prog, view, projection);

    prog.setUniform("useSpecular", true);

    car_->render();
}

void ShowcaseCar::renderDepth(GLSLProgram& depthProg)
{
    depthProg.setUniform("model", modelMatrix_);
    car_->render();
}

void ShowcaseCar::cleanup()
{
    for (int i = 0; i < COUNT; ++i)
    {
        if (carColours[i] != 0)
        {
            glDeleteTextures(1, &carColours[i]);
            carColours[i] = 0;
        }
    }

    for (GLuint tex : albedoTextures_)
    {
        if (tex != 0)
            glDeleteTextures(1, &tex);
    }

    if (normalTexture_ != 0)
    {
        glDeleteTextures(1, &normalTexture_);
        normalTexture_ = 0;
    }

    albedoTextures_.clear();

    car_.reset();
}

