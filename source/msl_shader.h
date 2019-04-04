#pragma once

#include "predefine.h"
#include "graphics_types.h"
#include "mtlpp.hpp"

#if EL_PLAT_IOS || EL_PLAT_OSX

class MSLShader final : public GraphicsShader
{
public:

	MSLShader();
	~MSLShader();

	void create(GraphicsShaderStageFlagBits stage, const char* shaderCode);
	void destroy();

private:

	mtlpp::Library _library;
	mtlpp::Function _function;
};

#endif
