#include "collision_object.h"
#include <glm/ext/matrix_transform.hpp>

#include "../helper/oriented_bounding_boxes.h"

using OrientedBoundingBoxes::OBB;
using OrientedBoundingBoxes::makeOBB;

CollisionObject::CollisionObject() : isActive_(true), data_{ false, glm::vec2(0.0f), 0.0f }, isSolid_(true) {}

int CollisionObject::id() const {
    return colliderId_;
}

glm::vec2 CollisionObject::collider_pos() const {
    return colliderPos_;
}

glm::vec2 CollisionObject::collider_size() const {
    return colliderSize_;
}

bool CollisionObject::collider_active() const {
    return isActive_;
}

bool CollisionObject::is_solid()
{
    return isSolid_;
}

void CollisionObject::update_collider_position(const glm::vec2& p) {
    colliderPos_ = p;
}

void CollisionObject::update_collider_angle(float r)
{
    // reveresed given angle because collision system expects counter-clockwise rot
    colliderAngle_ = -r;
}

void CollisionObject::set_collider_size(const glm::vec2& s) {

    // halving converts full size to half-extents for SAT calculations
    colliderSize_ = s * glm::vec2(0.5f);
}

void CollisionObject::set_is_solid(bool isSolid)
{
    isSolid_ = isSolid;
}

void CollisionObject::set_collider_active(bool a) {
    isActive_ = a;
}

OrientedBoundingBoxes::CollisionData CollisionObject::intersects(const CollisionObject& other) const {
    OBB A = makeOBB(colliderPos_, colliderSize_, colliderAngle_);
    OBB B = makeOBB(other.colliderPos_, other.colliderSize_, other.colliderAngle_);
    return OrientedBoundingBoxes::obbIntersect(A, B);
}

void CollisionObject::draw_collision_bounds(GLSLProgram& prog, glm::mat4& proj, glm::mat4& view)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(colliderPos_.x, 1, colliderPos_.y));
    model = glm::rotate(model, -colliderAngle_, glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
    model = glm::scale(model, glm::vec3(colliderSize_ * 2.f, 1));

    glm::mat4 mv = view * model;
    prog.setUniform("MVP", proj * mv);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
}

