#include "light_object.h"
#include <iostream>

using glm::vec4;

const glm::mat4 LightObject::SHADOW_BIAS = glm::mat4(
    vec4(0.5f, 0.0f, 0.0f, 0.0f),
    vec4(0.0f, 0.5f, 0.0f, 0.0f),
    vec4(0.0f, 0.0f, 0.5f, 0.0f),
    vec4(0.5f, 0.5f, 0.5f, 1.0f)
);

void LightObject::default_cleanup_light()
{
    if (shadowTex_ != 0) {
        glDeleteTextures(1, &shadowTex_);
        shadowTex_ = 0;
    }

    if (shadowFBO_ != 0) {
        glDeleteFramebuffers(1, &shadowFBO_);
        shadowFBO_ = 0;
    }

    shadowRes_ = 0;
    lightSpaceMatrix_ = glm::mat4(1.0f); // Reset to identity
}


void LightObject::initShadowMap(int res)
{
    shadowRes_ = res;

    GLfloat border[] = { 1.0f, 0.0f, 0.0f, 0.0f };

    glGenTextures(1, &shadowTex_);
    glBindTexture(GL_TEXTURE_2D, shadowTex_);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, shadowRes_, shadowRes_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

    glGenFramebuffers(1, &shadowFBO_);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex_, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is complete.\n");
    }
    else {
        printf("Framebuffer is not complete.\n");
    }

    std::cout << "[Spotlight " << this << "] FBO="
        << shadowFBO_ << " Tex=" << shadowTex_ << "\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 LightObject::get_light_space_matrix()
{
    return lightSpaceMatrix_;
}

GLuint LightObject::get_shadow_fbo()
{
    return shadowFBO_;
}

GLuint LightObject::get_shadow_tex()
{
    return shadowTex_;
}

int LightObject::get_shadow_res()
{
    return shadowRes_;
}
