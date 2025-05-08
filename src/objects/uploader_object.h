#pragma once
#include "../global_settings/global_settings_ubo/global_settings_ubo.h"

class UploaderObject 
{
public:
	virtual void upload(GlobalSettingsUBO& globalUbo) = 0;
};