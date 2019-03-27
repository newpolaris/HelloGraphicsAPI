#pragma once

#include <memory>
#include <vector>

#include "predefine.h"
#include "gl.h"
#include "mtlpp.hpp"

namespace el {

typedef std::shared_ptr<class GraphicsShader> GraphicsShaderPtr;
typedef std::shared_ptr<class GraphicsProgram> GraphicsProgramPtr;
typedef std::vector<GraphicsShaderPtr> GraphicsShaders;

// From vulkan spec.
enum GraphicsShaderStageFlagBits
{
	GraphicsShaderStageNoneBit = 0x00000000,
    GraphicsShaderStageVertexBit = 0x00000001,
    GraphicsShaderStageTessellationControlBit = 0x00000002,
    GraphicsShaderStageTessellationEvaluationBit = 0x00000004,
    GraphicsShaderStageGeometryBit = 0x00000008,
    GraphicsShaderStageFragmentBit = 0x00000010,
    GraphicsShaderStageComputeBit = 0x00000020,
    GraphicsShaderStageAllGraphics = 0x0000001F,
    GraphicsShaderStageAll = 0x7FFFFFFF,
    GraphicsShaderStageRaygenBit = 0x00000100,
    GraphicsShaderStageAnyHitBit = 0x00000200,
    GraphicsShaderStageClosestHitBit = 0x00000400,
    GraphicsShaderStageMissBit = 0x00000800,
    GraphicsShaderStageIntersectionBit = 0x00001000,
    GraphicsShaderStageCallableBit = 0x00002000,
    GraphicsShaderStageTaskBit = 0x00000040,
    GraphicsShaderStageMeshBit = 0x00000080,
    GraphicsShaderStageFlagBitsMaxEnum = 0x7FFFFFFF
};

namespace gl {
	namespace program
	{
		typedef GLuint Handle;
	}
}

class GraphicsShaderDesc final
{
public:

	GraphicsShaderDesc();

	void setStage(GraphicsShaderStageFlagBits stage);
	GraphicsShaderStageFlagBits getStage() const;

	void setShaderCode(const char* code);
	const char* getShaderCode() const;

private:

	GraphicsShaderStageFlagBits _stage;
	const char* _shaderCode;
};

class GraphicsShader
{
public:

	GraphicsShader();
	virtual ~GraphicsShader();

	virtual const GraphicsShaderDesc& getGraphicsShaderDesc() const noexcept = 0;

private:

	GraphicsShader(const GraphicsShader&) noexcept = delete;
	GraphicsShader& operator=(const GraphicsShader&) noexcept = delete;
};

class GLShader final : public GraphicsShader
{
public:

	GLShader();
	~GLShader();

	void create(GraphicsShaderStageFlagBits stage, const char* shaderCode);
	void destroy(gl::program::Handle program);

	GLuint getID() const;

	virtual const GraphicsShaderDesc& getGraphicsShaderDesc() const noexcept;

private:

	GLuint _id;
	GraphicsShaderStageFlagBits _stage;
	GraphicsShaderDesc _desc;
};

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

} // namespace el {
