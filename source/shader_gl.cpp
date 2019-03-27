#include "Shader_gl.h"
#include "gl.h"
#include "predefine.h"
#include "debug.h"
#include <vector>
#include <cassert>

namespace el {

GLenum gl::getShaderStage(GraphicsShaderStageFlagBits stage)
{
	switch (stage) {
	case GraphicsShaderStageVertexBit:
		return GL_VERTEX_SHADER;
	case GraphicsShaderStageFragmentBit:
		return GL_FRAGMENT_SHADER;
	default:
		assert(false);
	}
	return GraphicsShaderStageAll;
}

namespace shader {

	typedef GLuint Handle;

	const Handle kUninitialized = 0;

	bool isInitialized(const Handle& handle);
	Handle create(GLenum type, const char* shaderCode);
	void destroy(const el::gl::program::Handle& phandle, Handle& handle);

	bool isInitialized(const Handle& handle)
	{
		return handle != kUninitialized;
	}

	Handle create(GLenum type, const char* shaderCode)
	{
		GLuint id = gl::CreateShader(type);
		assert(id != 0);
		if (id != 0)
		{
			gl::ShaderSource(id, 1, &shaderCode, nullptr);
			gl::CompileShader(id);

			GLint compiled = 0;
			gl::GetShaderiv(id, GL_COMPILE_STATUS, &compiled);
			if (compiled == GL_FALSE)
			{
				GLint length = 0;
				gl::GetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
				std::vector<GLchar> buffer(length + 1);
				gl::GetShaderInfoLog(id, length, nullptr, buffer.data());
				el::trace("%s:%d %s\n", __FILE__, __LINE__, buffer.data());
				gl::DeleteShader(id);
				return 0;
			}
		}
		return Handle(id);
	}

	void destroy(const el::gl::program::Handle& phandle, Handle& handle)
	{
		if (isInitialized(handle))
		{
			gl::DetachShader(phandle, handle);
			gl::DeleteShader(handle);
			handle = 0;
		}
	}
} // namespace shader {

GraphicsShaderDesc::GraphicsShaderDesc() : _stage(GraphicsShaderStageFlagBitsMaxEnum)
{
}

void GraphicsShaderDesc::setStage(GraphicsShaderStageFlagBits stage)
{
	_stage = stage;
}

GraphicsShaderStageFlagBits GraphicsShaderDesc::getStage() const
{
	return _stage;
}

void GraphicsShaderDesc::setShaderCode(const char* code)
{
	_shaderCode = code;
}

const char* GraphicsShaderDesc::getShaderCode() const
{
	return _shaderCode;
}

GraphicsShader::GraphicsShader()
{
}

GraphicsShader::~GraphicsShader()
{
}

GLShader::GLShader() : _id(0)
{
}

GLShader::~GLShader()
{
}

void GLShader::create(GraphicsShaderStageFlagBits stage, const char* shaderCode)
{
	_stage = stage;
	_id = shader::create(gl::getShaderStage(stage), shaderCode);
}

void GLShader::destroy(gl::program::Handle program)
{
	shader::destroy(program, _id);
}

GLuint GLShader::getID() const
{
	return _id;
}

const GraphicsShaderDesc& GLShader::getGraphicsShaderDesc() const noexcept
{
	return _desc;
}

#if EL_PLAT_IOS || EL_PLAT_OSX

MSLShader::MSLShader()
{
}

MSLShader::~MSLShader()
{
}

void MSLShader::create(GraphicsShaderStageFlagBits stage, const char* shaderCode)
{
	mtlpp::Device device = mtlpp::Device::CreateSystemDefaultDevice();

    _library = device.NewLibrary(shaderCode, mtlpp::CompileOptions(), nullptr);
    assert(_library);

    _function = _library.NewFunction("vertFunc");
    assert(_function);
}

void MSLShader::destroy()
{
}

#endif // #if EL_PLAT_IOS || EL_PLAT_OSX


} // namespace el {
