#include "oriented_bounding_boxes.h"

using OrientedBoundingBoxes::OBB;

OBB OrientedBoundingBoxes::makeOBB(const glm::vec2& center, const glm::vec2& halfExtents, float angleRadians)
{
    OBB box;
    box.center = center;
    box.halfExtents = halfExtents;

    // uses given angle to get the OBBs y and x axis
    float c = cos(angleRadians), s = sin(angleRadians);
    box.axes[0] = glm::vec2(c, s);
    box.axes[1] = glm::vec2(-s, c);

    // generates the four corners of the OBB by combining positive / negative half - extents along the rotated axes
    for (int i = 0; i < 4; ++i) {
        // loops through all four corners of OBB
        glm::vec2 ext(
            (i == 0 || i == 3 ? 1.0f : -1.0f),
            (i < 2 ? 1.0f : -1.0f)
        );
        // multiplies the corner points by the half extent and the computed x and x axis
        box.corners[i] = center
            + ext.x * halfExtents.x * box.axes[0]
            + ext.y * halfExtents.y * box.axes[1];
    }
    return box;
}

// Separating Axis Theorem (SAT) core function
void OrientedBoundingBoxes::projectOntoAxis(const OBB& box, const glm::vec2& axis, float& outMin, float& outMax)
{
    // projects center onto axis. used as a midpoint of the OBBs point on the axis
    float centerProj = glm::dot(box.center, axis);
    
    // calculates total projection radius
    float extentProj = box.halfExtents.x * abs(dot(box.axes[0], axis)) + box.halfExtents.y * abs(dot(box.axes[1], axis));
    
    outMin = centerProj - extentProj; // the start of OBBs point on axis
    outMax = centerProj + extentProj; // the end of OBBs point on axis
}

bool OrientedBoundingBoxes::obbIntersect(const OBB& A, const OBB& B)
{
    float minA, maxA, minB, maxB;

    for (int i = 0; i < 2; ++i) {
        // tests against OBB As current axis
        projectOntoAxis(A, A.axes[i], minA, maxA); // projects A onto its own axis
        projectOntoAxis(B, A.axes[i], minB, maxB); // projects B onto As axis

        // returns if they dont overlap
        if (maxA < minB || maxB < minA) return false;

        // tests against OBB Bs current axis
        projectOntoAxis(B, B.axes[i], minB, maxB); // projects B onto its own axis
        projectOntoAxis(A, B.axes[i], minA, maxA); // projects A onto Bs axis

        // returns if they dont overlap
        if (maxA < minB || maxB < minA) return false;
    }

    // returns true because both axis overlap
    return true;
}
