#pragma once
#include "../scene_object.h"

#include "../../../helper/plane.h"
#include "../../../helper/glslprogram.h"
#include "../../mesh/tileable_plane.h"

class Floor : public SceneObject
{
public:
    Floor();
    ~Floor();

    void init() override;
    void update(float t) override;
    void render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) override;
    void renderDepth(GLSLProgram& depthProg) override;

private:
    TileablePlane plane_;
};