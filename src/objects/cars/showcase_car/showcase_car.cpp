#include "showcase_car.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

#include "../../../../helper/texture.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


void showcase_car::init(GLSLProgram& prog)
{
    prog_ = &prog;
    // load mesh
    car_ = ObjMesh::load("media/model/car.obj", true);

    // load textures
    diffuseColours[Orange] = Texture::loadTexture("media/texture/diffuse-orange.png");
    diffuseColours[Black] = Texture::loadTexture("media/texture/diffuse-black.png");
    diffuseColours[Blue] = Texture::loadTexture("media/texture/diffuse-blue.png");
    diffuseColours[DarkBlue] = Texture::loadTexture("media/texture/diffuse-darkblue.png");
    diffuseColours[DarkGrey] = Texture::loadTexture("media/texture/diffuse-darkgrey.png");
    diffuseColours[Grey] = Texture::loadTexture("media/texture/diffuse-grey.png");
    diffuseColours[Red] = Texture::loadTexture("media/texture/diffuse-red.png");

    normal_ = Texture::loadTexture("media/texture/normal.png");

    model_ = mat4(1.0f);
    model_ = glm::scale(model_, vec3(5.f));
    model_ = glm::translate(model_, vec3(0.0f, 0.8f, 0.0f));

    rotation_speed = 10.0f; // equivalent to 0.25f/frame at 60 FPS
    last_frame_time_ = glfwGetTime();
}

void showcase_car::update(float t)
{
    set_rotation(t);
    handle_texture_selection();
}

void showcase_car::set_rotation(float t)
{
    // saves last rot
    last_rot_ = rot_;

    // calculates delta time
    float delta_time = t - last_frame_time_;
    last_frame_time_ = t;

    // space key toggles animation
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!space_key_pressed_) {
            space_key_pressed_ = true;
            play_animation_ = !play_animation_;
        }
    } else {
        space_key_pressed_ = false;
    }

    // automatic rotation
    if (play_animation_) {
        rot_ += (is_negative_ ? -rotation_speed : rotation_speed) * delta_time;
    }

    // manual rotation controls
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
        rot_ += rotation_speed * delta_time;
        is_negative_ = false;
    }
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        rot_ -= rotation_speed * delta_time;
        is_negative_ = true;
    }
}

void showcase_car::handle_texture_selection()
{
    GLFWwindow* window = glfwGetCurrentContext();

    for (int key = GLFW_KEY_1; key <= GLFW_KEY_7; ++key)
    {
        if (glfwGetKey(window, key) == GLFW_PRESS)
        {
            const int textureIndex = key - GLFW_KEY_1;
            if (textureIndex >= 0 && textureIndex < COUNT)
            {
                 ColourIndex colour = static_cast<ColourIndex>(textureIndex);

                selectedColour1 = colour;

                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                    return;

                
                selectedColour2 = colour;
                return;
            }
        }
    }
}

void showcase_car::render(const mat4& view, const mat4& projection)
{
   // prog_->use();

    glActiveTexture(GL_TEXTURE0); // Unit 0 for Tex1
    glBindTexture(GL_TEXTURE_2D, diffuseColours[selectedColour1]);

    glActiveTexture(GL_TEXTURE1); // Unit 1 for Tex2
    glBindTexture(GL_TEXTURE_2D, diffuseColours[selectedColour2]);

    glActiveTexture(GL_TEXTURE2); // Unit 2 for NormalTex
    glBindTexture(GL_TEXTURE_2D, normal_);

    prog_->setUniform("Material.Kd", vec3(0.7f, 0.7f, 0.7f));
    prog_->setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
    prog_->setUniform("Material.Ka", vec3(0.2f, 0.2f, 0.2f));
    prog_->setUniform("Material.Shininess", 25.f);

    prog_->setUniform("TextureScaleX", 1.0f);
    prog_->setUniform("TextureScaleY", 1.0f);

    // set up model matrix
    model_ = mat4(1.0f);
    model_ = glm::scale(model_, vec3(5.f));
    model_ = glm::translate(model_, vec3(0.0f, 0.8f, 0.0f));
    model_ = glm::rotate(model_, glm::radians(rot_), vec3(0.0f, 1.0f, 0.0f));

    // send matrices
    mat4 mv = view * model_;
    prog_->setUniform("ModelViewMatrix", mv);
    prog_->setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog_->setUniform("MVP", projection * mv);

    prog_->setUniform("useSpecular", true);


    car_->render();
}

