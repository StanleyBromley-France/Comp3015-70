#pragma once
#include "../../../../helper/objmesh.h"
#include "../../../../helper/glslprogram.h"
#include "../../scene_object.h"
class ShowcaseCar : public SceneObject
{
public:
    void init() override;
    void update(float t) override;
    void render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) override;

private:
    std::unique_ptr<ObjMesh> car_;
    float rot_ = 45.0f;
    float last_rot_ = 0;
    
    void set_rotation(float t);
    void handle_texture_selection();

    // rot settings
    float rotationSpeed_;
    bool doRotation_ = true;
    bool rotatingClockwise_ = false;
    double lastFrameTime_; 
    float lastRot_ = 0;

    enum ColourIndex {
        Orange, Black, Blue, DarkBlue, DarkGrey, Grey, Red, COUNT
    };

    GLuint carColours[COUNT];
};
