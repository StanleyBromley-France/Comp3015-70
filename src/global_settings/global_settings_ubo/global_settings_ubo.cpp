#include "global_settings_ubo.h"

GlobalSettingsUBO::GlobalSettingsUBO()
    : ubo_(0), dirty_(false)
{

    // allocate and bind UBO
    glGenBuffers(1, &ubo_);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Data), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GlobalSettingsUBO::~GlobalSettingsUBO() {
    if (ubo_) glDeleteBuffers(1, &ubo_);
}

void GlobalSettingsUBO::setSpotlight(const SpotLightInfo& spot) {
    data.Spotlight = spot;
    dirty_ = true;
}

void GlobalSettingsUBO::setLightingMode(int mode) {
    data.lightingMode = mode;
    dirty_ = true;
}

void GlobalSettingsUBO::updateGPU() {
    if (!dirty_) return;
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Data), &data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    dirty_ = false;
}
