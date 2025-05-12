#include "game_checkpoint.h"
#include "../../game_management/game_manager.h"
#include <iostream>

void GameCheckpoint::init()
{
    std::cout << "creating checkpoint \n";
    update_collider_position(glm::vec2(position_.x, position_.z));
    set_collider_size(glm::vec2(15.f, 3.f));
    update_collider_angle(rotation_);
    set_is_solid(false);
}

void GameCheckpoint::update(float t)
{
}

void GameCheckpoint::render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog)
{
}

void GameCheckpoint::renderDepth(GLSLProgram& depthProg)
{
}

void GameCheckpoint::on_collision(CollisionObject& other)
{
    GameManager::instance().arrive_at_checkpoint(checkpointId_);
}

void GameCheckpoint::on_collision_once(CollisionObject& other)
{
}
