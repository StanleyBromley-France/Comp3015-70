#pragma once

#include <glm/glm.hpp>

namespace OrientedBoundingBoxes
{
    struct CollisionData {
        bool hasCollided;
        glm::vec2 normal;
        float depth;
    };

    struct OBB {
        glm::vec2 center;
        glm::vec2 halfExtents;
        glm::vec2 axes[2];
        glm::vec2 corners[4];
    };

    OBB makeOBB(const glm::vec2& center, const glm::vec2& halfExtents, float angleRadians);
    void projectOntoAxis(const OBB& box, const glm::vec2& axis, float& outMin, float& outMax);
    CollisionData obbIntersect(const OBB& A, const OBB& B);
}