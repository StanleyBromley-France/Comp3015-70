#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include <glad/glad.h>
#include "../../../helper/glslprogram.h"

class post_processor {
public:
    // Access the singleton instance.
    static post_processor& get_instance();

    post_processor(const post_processor&) = delete;
    post_processor& operator=(const post_processor&) = delete;
    post_processor(post_processor&&) = delete;
    post_processor& operator=(post_processor&&) = delete;

    void begin_scene_capture() const;
    
    // Called every frame to execute the post processing passes.
    void apply_post_render();

    // Resize FBO and textures if the window size changes.
    void resize(int screen_width, int screen_height);

private:
    post_processor();
    ~post_processor();

    // helpers
    void compile_shaders();
    void setup_fbo();
    void setup_quad();
    void set_gaussian_weights();
    void setup_samplers();

    // post-processing passes
    void pass2_brightness_threshold();
    void pass3_vertical_blur();
    void pass4_horizontal_blur();
    void pass5_tone_mapping();

    static float gauss(float, float);
    void compute_log_ave_luminance();
    

    // FBO & texture handles
    GLuint fs_quad_;
    GLuint hdr_fbo_, blur_fbo_, log_lum_fbo_;
    GLuint hdr_tex_, tex1_, tex2_, log_lum_tex_;
    GLuint linear_sampler_, nearest_sampler_;
    int bloom_buf_width_, bloom_buf_height_;
    int width_, height_;
    
    // GLSL shader program for post-processing
    GLSLProgram post_prog_;
    glm::mat4 model_;
    glm::mat4 view_;
    glm::mat4 projection_;
};

#endif // POSTPROCESSING_H
