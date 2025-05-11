#pragma once
#include "../../../helper/glslprogram.h"

class CollisionDebugRenderer {
public:
    CollisionDebugRenderer();
    ~CollisionDebugRenderer();

    GLuint getVAO() const;

private:
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
};
