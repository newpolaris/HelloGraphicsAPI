#pragma once

#include "predefine.h"
#include <graphics_types.h>
#include <linmath.h>

namespace el {

	class GraphicsContext
	{
	public:

		GraphicsContext();
		virtual ~GraphicsContext();

        virtual void setViewport(const Viewport& viewport) = 0;
        virtual void setProgram(const GraphicsProgramPtr& program) = 0;
        virtual void setTexture(const std::string& name, const GraphicsTexturePtr& texture) = 0;
        virtual void setVertexBuffer(const std::string& name, const GraphicsBufferPtr& vertex_buffer, uint32_t stride, uint32_t offset) = 0;
        virtual void setUniform(const std::string& name, const vec3& v0) = 0;
        virtual void setUniform(const std::string& name, const mat4x4& m0) = 0;

	private:

		GraphicsContext(const GraphicsContext&) = delete;
		GraphicsContext& operator=(const GraphicsContext&) = delete;
	};

} // namespace el {
