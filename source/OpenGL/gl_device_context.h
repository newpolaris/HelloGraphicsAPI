#pragma once

#include "predefine.h"

#if EL_BUILD_OPENGL

#include "graphics_device_context.h"

namespace el {

	class GLDeviceContext final : public GraphicsDeviceContext
	{
	public:

		GLDeviceContext();
		~GLDeviceContext();

        bool create();
        void destory();
        
	private:

	};

} // namespace el {

#endif // #if EL_BUILD_OPENGL
