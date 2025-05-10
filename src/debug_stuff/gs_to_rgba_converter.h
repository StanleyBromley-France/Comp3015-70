#pragma once

#include"../../helper/glslprogram.h"


class GrayscaleToRGBAConverter {
public:
    GrayscaleToRGBAConverter(int width = 1024, int height = 1024);
    ~GrayscaleToRGBAConverter();

    // Convert the single-channel texture to RGBA; returns the RGBA tex ID
    GLuint convert(GLuint grayscaleTex);

private:
    void initFramebuffer();
    void initShader();

    int width_, height_;
    GLuint fbo_ = 0;
    GLuint quadVAO_, quadVBO_;
    GLuint rgbaTex_ = 0;
    GLSLProgram prog_;
};