#include "particle_spawner.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include "../../../../helper/glslprogram.h"
#include "../../../../helper/particleutils.h"

ParticleSpawner::ParticleSpawner()
    : initVelBuf_(0)
    , startTimeBuf_(0)
    , vao_(0)
    , textureID_(0)
    , nParticles_(0)
    , particleLifetime_(0.0f)
    , emitterPos_(0.0f)
    , emitterDir_(0.0f)
    , particleSize_(0.0f)
    , gravity_(0.0f)
    , prog_(nullptr)
    , time_(0.0f)
    , armed_(false)
    , triggerTime_(0.0f)
{}

ParticleSpawner::~ParticleSpawner()
{
    if (initVelBuf_)    glDeleteBuffers(1, &initVelBuf_);
    if (startTimeBuf_)  glDeleteBuffers(1, &startTimeBuf_);
    if (vao_)           glDeleteVertexArrays(1, &vao_);
}

void ParticleSpawner::init(GLSLProgram& prog,
    GLuint texID,
    unsigned count,
    float lifetime,
    const glm::vec3& emitterPos,
    const glm::vec3& emitterDir,
    float size)
{
    prog_ = &prog;
    textureID_ = texID;
    nParticles_ = count;
    particleLifetime_ = lifetime;
    emitterPos_ = emitterPos;
    emitterDir_ = emitterDir;
    particleSize_ = size;
    gravity_ = glm::vec3(0.0f, -1.f, 0.0f);
    time_ = 0.0f;

    tempData_.resize(std::max(count * 3u, count));

    glGenBuffers(1, &initVelBuf_);
    glGenBuffers(1, &startTimeBuf_);
    glGenVertexArrays(1, &vao_);

    initBuffers();

    prog_->use();
    prog_->setUniform("ParticleTex", 0);
    prog_->setUniform("ParticleLifetime", particleLifetime_);
    prog_->setUniform("ParticleSize", particleSize_);
    prog_->setUniform("Gravity", gravity_);
    prog_->setUniform("EmitterPos", emitterPos_);
}

void ParticleSpawner::initBuffers()
{
    size_t velBytes = nParticles_ * sizeof(GLfloat) * 3;
    size_t timeBytes = nParticles_ * sizeof(GLfloat);

    // fill init-vel buffer
    glm::mat3 basis = ParticleUtils::makeArbitraryBasis(emitterDir_);
    for (unsigned i = 0; i < nParticles_; ++i) {
        float theta = glm::mix(0.0f, glm::pi<float>() / 20.0f, randFloat());
        float phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());
        float speed = glm::mix(10.f, 14.f, randFloat());

        glm::vec3 v;
        v.x = sinf(theta) * cosf(phi);
        v.y = cosf(theta);
        v.z = sinf(theta) * sinf(phi);
        v = glm::normalize(basis * v) * speed;

        tempData_[3 * i + 0] = v.x;
        tempData_[3 * i + 1] = v.y;
        tempData_[3 * i + 2] = v.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, initVelBuf_);
    glBufferData(GL_ARRAY_BUFFER, velBytes, tempData_.data(), GL_STATIC_DRAW);

    // fill start-time buffer
    float rate = particleLifetime_ / float(nParticles_);
    for (unsigned i = 0; i < nParticles_; ++i)
        tempData_[i] = rate * float(i);

    glBindBuffer(GL_ARRAY_BUFFER, startTimeBuf_);
    glBufferData(GL_ARRAY_BUFFER, timeBytes, tempData_.data(), GL_STATIC_DRAW);

    // setup VAO
    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, initVelBuf_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribDivisor(0, 1);

    glBindBuffer(GL_ARRAY_BUFFER, startTimeBuf_);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleSpawner::update(float currentTime)
{
    if (armed_)
    {
        float elapsed = currentTime - triggerTime_;
        float maxLifetime = particleLifetime_ + (particleLifetime_ / float(nParticles_)) * float(nParticles_);
        time_ = fmod(elapsed, maxLifetime);
    }
    else 
    {
        time_ = -particleLifetime_;
    }
}

void ParticleSpawner::render(const glm::mat4& view,
    const glm::mat4& projection)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID_);
    glBindVertexArray(vao_);

    prog_->setUniform("Time", time_);
    prog_->setUniform("MV", view * glm::mat4(1.0f));
    prog_->setUniform("Proj", projection);

    // disables depth writes so blended particles composite properly
    glDepthMask(GL_FALSE);

    // enables blending for the alpha 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles_);

    // re-enable changed stuff
    glDepthMask(GL_TRUE);

    glBindVertexArray(0);

    glDisable(GL_BLEND);
}
float ParticleSpawner::randFloat()
{
    return rand_.nextFloat();
}

void ParticleSpawner::fire(float currentTime) {
    armed_ = true;
    triggerTime_ = currentTime;
}