#pragma once

#include "../../helper/glslprogram.h"
#include <memory>
#include "../../helper/objmesh.h"

class Object {
public:
    virtual void init(GLSLProgram& prog) = 0;
    virtual void update(float t) = 0;
    virtual void render(const glm::mat4& view, const glm::mat4& projection) = 0;
protected:
    GLSLProgram* prog_;
    std::unique_ptr<ObjMesh> mesh_;
};