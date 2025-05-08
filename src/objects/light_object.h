#pragma once
#include "../../helper/glslprogram.h"
#include "../global_settings/global_settings_ubo/global_settings_ubo.h"
#include "../global_settings/lights/spotlight/spotlight.h"

class LightObject 
{
public:
    void initShadowMap(int resolution = 1024);
    glm::mat4 get_light_space_matrix();

    GLuint get_shadow_fbo();
    GLuint get_shadow_tex();
    int get_shadow_res();

    virtual void calculate_light_space_matrix() = 0;
	virtual void render_light(const glm::mat4& view, const glm::mat4& projection) = 0;
protected:
    GLuint shadowFBO_;
    GLuint shadowTex_;
    int shadowRes_;
    glm::mat4 lightSpaceMatrix_;
};