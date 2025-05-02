#include <iostream>
#include <sstream>
#include <vector>

#include "post_process_manager.h"

#include <glm/gtc/constants.hpp>

#include "../../../helper/glutils.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

post_processor& post_processor::get_instance() {
    static post_processor instance;
    return instance;
}

post_processor::post_processor() : fs_quad_(0), hdr_fbo_(0), blur_fbo_(0), log_lum_fbo_(0), hdr_tex_(0), tex1_(0),
		tex2_(0), log_lum_tex_(0), linear_sampler_(0), nearest_sampler_(0), width_(800), height_(600)
{
	compile_shaders();
	setup_fbo();
	setup_quad();
	set_gaussian_weights();
	setup_samplers();
	model_ = mat4(1.0f);
	view_ = mat4(1.0f);
	projection_ = mat4(1.0f);

	mat4 mv = view_ * model_;
	post_prog_.setUniform("ModelViewMatrix", mv);
	post_prog_.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	post_prog_.setUniform("MVP", projection_ * mv);
}

post_processor::~post_processor() {
    glDeleteFramebuffers(1, &hdr_fbo_);
    glDeleteFramebuffers(1, &blur_fbo_);
    glDeleteTextures(1, &hdr_tex_);
    glDeleteTextures(1, &tex1_);
    glDeleteTextures(1, &tex2_);
    glDeleteVertexArrays(1, &fs_quad_);
	glDeleteFramebuffers(1, &log_lum_fbo_);
	glDeleteTextures(1, &log_lum_tex_);
}

void post_processor::resize(int screen_width, int screen_height) {
    width_ = screen_width;
    height_ = screen_height;
    glViewport(0, 0, width_, height_);
    // Re-create or resize the FBO and textures.
    setup_fbo();
}

void post_processor::compile_shaders() {
	try {
		post_prog_.compileShader("shader/post_processing/post_process.vert");
		post_prog_.compileShader("shader/post_processing/post_process.frag");
		post_prog_.link();
		post_prog_.use();
	}
	catch (GLSLProgramException &e) {
		std::cout << e.what() << '\n';
		exit(EXIT_FAILURE);
	}
}


void post_processor::setup_fbo() {
	// Generate and bind the framebuffer
	glGenFramebuffers(1, &hdr_fbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, hdr_fbo_);
	// Create the texture object
	glGenTextures(1, &hdr_tex_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdr_tex_);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, width_, height_);
	// Bind the texture to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdr_tex_, 0);
	// Create the depth buffer
	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width_, height_);
	// Bind the depth buffer to the FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuf);
	// Set the targets for the fragment output variables
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	// Create an FBO for the bright-pass filter and blur
	glGenFramebuffers(1, &blur_fbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, blur_fbo_);
	// Create two texture objects to ping-pong for the bright-pass filter
	// and the two-pass blur
	bloom_buf_width_ = width_ / 8;
	bloom_buf_height_ = height_ / 8;
	glGenTextures(1, &tex1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1_);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, bloom_buf_width_, bloom_buf_height_);
	glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &tex2_);
	glBindTexture(GL_TEXTURE_2D, tex2_);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, bloom_buf_width_, bloom_buf_height_);
	// Bind tex1 to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1_, 0);
	glDrawBuffers(1, drawBuffers);
	// Unbind the framebuffer, and revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Create texture for log luminance
	glGenTextures(1, &log_lum_tex_);
	glActiveTexture(GL_TEXTURE3); // Assuming 3 is unused
	glBindTexture(GL_TEXTURE_2D, log_lum_tex_);
	glTexStorage2D(GL_TEXTURE_2D, static_cast<GLint>(std::log2(std::max(width_, height_))) + 1, GL_R32F, width_, height_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Create framebuffer for Pass 1
	glGenFramebuffers(1, &log_lum_fbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, log_lum_fbo_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, log_lum_tex_, 0);
	glDrawBuffers(1, drawBuffers); // reuse previous drawBuffers

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void post_processor::setup_quad() {
	constexpr GLfloat verts[] = {
		-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
	};
	constexpr GLfloat tc[] = {
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};

	GLuint handle[2];
	glGenBuffers(2, handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tc), tc, GL_STATIC_DRAW);

	glGenVertexArrays(1, &fs_quad_);
	glBindVertexArray(fs_quad_);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void post_processor::set_gaussian_weights() {
	post_prog_.setUniform("LumThresh", 1.7f);

	float weights[10];
	constexpr float sigma2 = 25.0f;
	weights[0] = gauss(0, sigma2);
	float sum = weights[0];

	for (int i = 1; i < 10; ++i) {
		weights[i] = gauss(static_cast<float>(i), sigma2);
		sum += 2 * weights[i];
	}

	for (int i = 0; i < 10; ++i) {
		std::stringstream uniName;
		uniName << "Weight[" << i << "]";
		post_prog_.setUniform(uniName.str().c_str(), weights[i] / sum);
	}
}

void post_processor::setup_samplers() {
	GLuint samplers[2];
	glGenSamplers(2, samplers);
	linear_sampler_ = samplers[0];
	nearest_sampler_ = samplers[1];

	constexpr GLfloat border[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glSamplerParameteri(nearest_sampler_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(nearest_sampler_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(nearest_sampler_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glSamplerParameteri(nearest_sampler_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glSamplerParameterfv(nearest_sampler_, GL_TEXTURE_BORDER_COLOR, border);

	glSamplerParameteri(linear_sampler_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(linear_sampler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(linear_sampler_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glSamplerParameteri(linear_sampler_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glSamplerParameterfv(linear_sampler_, GL_TEXTURE_BORDER_COLOR, border);

	glBindSampler(0, nearest_sampler_);
	glBindSampler(1, nearest_sampler_);
	glBindSampler(2, nearest_sampler_);
}


float post_processor::gauss(float x, float sigma2)
{
	const double coeff = 1.0 / (glm::two_pi<float>() * sigma2);
	const double exponent = -(x * x) / (2.0 * sigma2);
	return static_cast<float>(coeff * exp(exponent));
}

void post_processor::begin_scene_capture() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, hdr_fbo_);
	glViewport(0, 0, width_, height_);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void post_processor::compute_log_ave_luminance() {
	post_prog_.setUniform("Pass", 1);

	glBindFramebuffer(GL_FRAMEBUFFER, log_lum_fbo_);
	glViewport(0, 0, width_, height_);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	glBindVertexArray(fs_quad_);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// generate mipmaps
	// this creates an average of all pixel log-luminance values
	glBindTexture(GL_TEXTURE_2D, log_lum_tex_);
	glGenerateMipmap(GL_TEXTURE_2D);

	float ave_log_lum = 0.0f;
	int mip_level = static_cast<int>(std::log2(std::max(width_, height_)));
	glBindTexture(GL_TEXTURE_2D, log_lum_tex_);
	glGetTexImage(GL_TEXTURE_2D, mip_level, GL_RED, GL_FLOAT, &ave_log_lum);

	float ave_lum = expf(ave_log_lum);
	post_prog_.use();
	post_prog_.setUniform("AveLum", ave_lum);
}

void post_processor::pass2_brightness_threshold()
{
	post_prog_.setUniform("Pass", 2);
	glBindFramebuffer(GL_FRAMEBUFFER, blur_fbo_);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1_, 0);
	glViewport(0, 0, bloom_buf_width_, bloom_buf_height_);
	glDisable(GL_DEPTH_TEST);

	glClearColor(0, 0, 0, 0);

	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(fs_quad_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void post_processor::pass3_vertical_blur() {
	post_prog_.setUniform("Pass", 3);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex2_, 0);

	glBindVertexArray(fs_quad_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void post_processor::pass4_horizontal_blur() {
	post_prog_.setUniform("Pass", 4);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1_, 0);

	glBindVertexArray(fs_quad_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void post_processor::pass5_tone_mapping() {
	post_prog_.setUniform("Pass", 5);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width_, height_);

	glBindSampler(1, linear_sampler_);
	glBindVertexArray(fs_quad_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindSampler(1, nearest_sampler_);
}

void post_processor::apply_post_render() {
    
    post_prog_.use();
    
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdr_tex_);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1_);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex2_);

    // compute average luminance from the rendered HDR texture.
    compute_log_ave_luminance();
   

    // execute the post-processing passes.
    pass2_brightness_threshold();
    pass3_vertical_blur();
    pass4_horizontal_blur();
    pass5_tone_mapping();
}
