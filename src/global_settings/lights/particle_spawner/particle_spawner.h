#pragma once

#include "../../../../helper/random.h"
#include "../../../../helper/glslprogram.h"

class ParticleSpawner {
public:
    ParticleSpawner();
    ~ParticleSpawner();

    void init(GLSLProgram& prog,
        GLuint texture,
        unsigned count,
        float lifetime,
        const glm::vec3& emitterPos,
        const glm::vec3& emitterDir,
        float size);

    void update(float currentTime);

    void render(const glm::mat4& view,
        const glm::mat4& projection);

    void fire(float currentTime);
private:
    void initBuffers();
    float randFloat();

    GLuint initVelBuf_;
    GLuint startTimeBuf_;
    GLuint vao_;
    GLuint textureID_;

    unsigned   nParticles_;
    float      particleLifetime_;
    glm::vec3  emitterPos_;
    glm::vec3  emitterDir_;
    float      particleSize_;
    glm::vec3  gravity_;

    GLSLProgram* prog_;

    float time_;
    bool armed_;
    float triggerTime_;

    std::vector<GLfloat> tempData_;

    Random rand_;
};