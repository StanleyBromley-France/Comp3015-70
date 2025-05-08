#pragma once
#include "../../helper/glslprogram.h"

class ParticleObject 
{
public:
	virtual void renderParticles(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog) = 0;
};