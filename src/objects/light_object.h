#pragma once
#include "../../helper/glslprogram.h"

class LightObject 
{
public:
	virtual void renderLight(const glm::mat4& view, const glm::mat4& projection) = 0;
};