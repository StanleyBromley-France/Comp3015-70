#pragma once
#include "../object.h"

#include "../../../helper/plane.h"
#include "../../../helper/glslprogram.h"
#include "../../mesh/tileable_plane.h"

class Floor : Object 
{
public:
    Floor();
    void init(GLSLProgram& prog) override;
    void update(float t) override;
    void render(const glm::mat4& view, const glm::mat4& projection) override;
private:
    GLSLProgram* prog_;
    GLuint normal_;
    GLuint diffuse_;
    glm::mat4 model_;

    TileablePlane plane_;
    Plane plane2_;
};