#pragma once
#include "../../../../helper/objmesh.h"
#include "../../../../helper/glslprogram.h"
#include "../../object.h"
class showcase_car : Object
{
public:
    void init(GLSLProgram& prog) override;
    void update(float t) override;
    void render(const glm::mat4& view, const glm::mat4& projection) override;

private:
    std::unique_ptr<ObjMesh> car_;
    float rot_ = 45.0f;
    float last_rot_ = 0;
    glm::mat4 model_;
    
    void set_rotation(float t);
    void handle_texture_selection();

    // rot settings
    float rotation_speed;
    bool play_animation_ = true;
    bool space_key_pressed_ = false;
    bool is_negative_ = false;
    double last_frame_time_; 

    GLuint normal_;

    enum ColourIndex {
        Orange, Black, Blue, DarkBlue, DarkGrey, Grey, Red, COUNT
    };

    GLuint diffuseColours[COUNT];
    ColourIndex selectedColour1 = Orange;
    ColourIndex selectedColour2 = Orange;

};
