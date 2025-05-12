#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include "../../../helper/glslprogram.h"
#include "../helper/oriented_bounding_boxes.h"

using OrientedBoundingBoxes::CollisionData;

class CollisionObject {
    friend class CollisionManager; // allows collision manager to assign ids
public:
    CollisionObject();
    ~CollisionObject() = default;

    int id() const;
    glm::vec2 collider_pos() const;
    glm::vec2 collider_size() const;
    bool collider_active() const;

    void set_collider_active(bool a);
    CollisionData intersects(const CollisionObject& other) const;
    void draw_collision_bounds(GLSLProgram& prog, glm::mat4& proj, glm::mat4& view);

    const CollisionData& collision_data() const { return data_; }
    void set_collision_data(const CollisionData& manifold) { data_ = manifold; }

    bool is_solid();
protected:
    void update_collider_position(const glm::vec2& p);
    void update_collider_angle(float r);

    virtual void on_collision(CollisionObject& other) = 0;
    virtual void on_collision_once(CollisionObject& other) = 0;

    void set_collider_size(const glm::vec2& s);
    void set_is_solid(bool);
    bool isActive_;

    CollisionData data_;

private:
    int colliderId_;
    glm::vec2 colliderPos_;
    glm::vec2 colliderSize_;
    float colliderAngle_ { 0.0f };

    bool isSolid_;
};