#include "program_gl.h"

#include "gl.h"
#include "debug.h"

namespace el {
namespace gl {
	typedef GLuint Handle;
	const Handle kUninitialized = Handle(0);

	bool isInitialized(const Handle& h);
	Handle create(const GLuint vertex, const GLuint fragment);
	void destroy(Handle& handle);


	bool isInitialized(const Handle& h)
	{
		return h != kUninitialized;
	}

	Handle create(const GLuint vertex, const GLuint fragment)
	{
		GLuint id = gl::CreateProgram();

		GLint status = 0;
		if (vertex != 0)
		{
			gl::AttachShader(id, vertex);
			if (fragment != 0)
				gl::AttachShader(id, fragment);
			gl::LinkProgram(id);
			GL_CHECK(glGetProgramiv(id, GL_LINK_STATUS, &status));

			if (status == GL_FALSE || EL_CONFIG_DEBUG)
			{
				const uint32_t kBufferSize = 512u;
				char log[kBufferSize];
				GL_CHECK(glGetProgramInfoLog(id, sizeof(log), nullptr, log));
				EL_TRACE("%d: %s", status, log);
			}
		}

		if (status == GL_FALSE)
		{
			gl::DeleteProgram(id);
			id = 0;
		}
		return Handle(id);
	}

	void destroy(Handle& handle)
	{
		if (isInitialized(handle))
		{
			gl::DeleteProgram(handle);
			handle = 0;
		}
	}

} // namespace gl {

GraphicsProgram::GraphicsProgram()
{
}

GraphicsProgram::~GraphicsProgram()
{
}

GraphicsProgramDesc::GraphicsProgramDesc()
{
}

void GraphicsProgramDesc::addShader(GraphicsShaderPtr ptr) 
{
	_shaders.push_back(std::move(ptr));
}

const GraphicsShaders& GraphicsProgramDesc::getShaders() const
{
	return _shaders;
}

GLProgram::GLProgram() : _id(0)
{
}

GLProgram::~GLProgram()
{
}

bool GLProgram::create(const GraphicsProgramDesc& desc)
{
	// GLuint shaders[GraphicsShaderStageAll];
	for (auto& s : desc.getShaders())
	{
		auto shader = std::static_pointer_cast<GLShader>(s);
		// gl::create()
	}

	_desc = desc;

	return true;
}

void GLProgram::destory()
{
}

GLuint GLProgram::GetID() const
{
	return _id;
}

const GraphicsProgramDesc & GLProgram::getGraphicsProgramDesc() const noexcept
{
	return _desc;
}
} // namespace el {

