//========================================================================
// Context sharing example
// Copyright (c) Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#include "predefine.h"
#include "debug.h"
#include "gl.h"

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <array>
#include <chrono>

#include "getopt.h"
#include "linmath.h"

#include "graphics_device.h"
#include "graphics_texture.h"
#include "graphics_types.h"
#include "graphics_shader.h"
#include "graphics_program.h"

#include "gl_buffer.h"

// TODO:
#include "gl_program.h"
#include "gl_shader.h"
#include "gl_texture.h"

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec2 vPos;\n"
"varying vec2 texcoord;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    texcoord = vPos;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"uniform sampler2D texture;\n"
"uniform vec3 color;\n"
"varying vec2 texcoord;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color * texture2D(texture, texcoord).rgb, 1.0);\n"
"}\n";

static const vec2 vertices[4] =
{
    { 0.f, 0.f },
    { 1.f, 0.f },
    { 1.f, 1.f },
    { 0.f, 1.f }
};

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void APIENTRY gl_debug_callback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	// ignore these non-significant error codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204 || id == 131184) 
		return;

	std::stringstream output;
	output << "---------- OPENGL CALLBACK -----------" << std::endl;
	output << "SOURCE: ";
	switch (source) {
	case GL_DEBUG_SOURCE_API:
		output << "WINDOW_SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		output << "SHADER_COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		output << "THIRD_PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		output << "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		output << "OTHER";
		break;
	}
	output << std::endl;

	output << "TYPE: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		output << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		output << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		output << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		output << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		output << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		output << "OTHER";
		break;
	}
	output << std::endl;

	output << "SEVERITY : ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		output << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		output << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		output << "HIGH";
		break;
	}
	output << std::endl;
	output << message << std::endl;

	EL_TRACE("%s", output.str().c_str());
}

class ProfileBusyWait final
{
public:

	ProfileBusyWait();
	~ProfileBusyWait();
};

int main(int argc, char** argv)
{
    GLFWwindow* windows[2];
    GLint mvp_location, vpos_location, color_location, texture_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    windows[0] = glfwCreateWindow(400, 400, "First", NULL, NULL);
    if (!windows[0])
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(windows[0], key_callback);

    glfwMakeContextCurrent(windows[0]);

    // Only enable vsync for the first of the windows to be swapped to
    // avoid waiting out the interval for each window
    glfwSwapInterval(1);

    // The contexts are created with the same APIs so the function
    // pointers should be re-usable between them
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

#if _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	if (glDebugMessageCallback) {
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		glDebugMessageCallback(gl_debug_callback, nullptr);
	}
#endif

	using namespace el;

	GraphicsDeviceDesc deviceDesc;
	deviceDesc.setType(GraphicsDeviceTypeOpenGL);

	GraphicsDevicePtr device = createDevice(deviceDesc);

	GraphicsShaderPtr vertex_shader;
	GraphicsShaderPtr fragment_shader;
	GraphicsProgramPtr program;
	GraphicsTexturePtr texture;

	GraphicsBufferPtr vertex_buffer;

    // Create the OpenGL objects inside the first context, created above
    // All objects will be shared with the second context, created below
    {
		GraphicsTextureDesc texture_desc;
		texture_desc.setTarget(GraphicsTextureTarget2D);
		texture_desc.setWidth(16);
		texture_desc.setHeight(16);

		texture = device->createTexture(texture_desc);

		GraphicsShaderDesc vertex_desc;
		vertex_desc.setStageFlag(GraphicsShaderStageVertexBit);
		vertex_desc.setShaderCode(vertex_shader_text);

		GraphicsShaderDesc fragment_desc;
		fragment_desc.setStageFlag(GraphicsShaderStageFragmentBit);
		fragment_desc.setShaderCode(fragment_shader_text);

		vertex_shader = device->createShader(vertex_desc);
		fragment_shader = device->createShader(fragment_desc);

		GraphicsProgramDesc program_desc;
		program_desc.addShader(vertex_shader);
		program_desc.addShader(fragment_shader);

		program = device->createProgram(program_desc);
		auto program_id = std::static_pointer_cast<GLProgram>(program)->getProgramID();

        mvp_location = glGetUniformLocation(program_id, "MVP");
        color_location = glGetUniformLocation(program_id, "color");
        texture_location = glGetUniformLocation(program_id, "texture");
        vpos_location = glGetAttribLocation(program_id, "vPos");

		GraphicsBufferDesc vertices_buffer_desc;
		vertices_buffer_desc.setData((const char*)vertices);
		vertices_buffer_desc.setDataSize(sizeof(vertices));

		vertex_buffer = device->createBuffer(vertices_buffer_desc);
    }

	const GLint slot0 = 0;

	auto gl_program = std::static_pointer_cast<GLProgram>(program);
	gl_program->use();
	gl_program->setTexture(texture_location, texture, slot0);
	gl_program->setVertexBuffer(vpos_location, vertex_buffer, 2, GL_FLOAT, sizeof(vertices[0]), 0);

    windows[1] = glfwCreateWindow(400, 400, "Second", NULL, windows[0]);
    if (!windows[1])
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Place the second window to the right of the first
    {
        int xpos, ypos, left, right, width;

        glfwGetWindowSize(windows[0], &width, NULL);
        glfwGetWindowFrameSize(windows[0], &left, NULL, &right, NULL);
        glfwGetWindowPos(windows[0], &xpos, &ypos);

        glfwSetWindowPos(windows[1], xpos + width + left + right, ypos);
    }

    glfwSetKeyCallback(windows[1], key_callback);

    glfwMakeContextCurrent(windows[1]);

    // While objects are shared, the global context state is not and will
    // need to be set up for each context
	gl_program->use();
	gl_program->setTexture(texture_location, texture, slot0);
	gl_program->setVertexBuffer(vpos_location, vertex_buffer, 2, GL_FLOAT, sizeof(vertices[0]), 0);

	// gl_program->setIndexBuffer()

    while (!glfwWindowShouldClose(windows[0]) &&
           !glfwWindowShouldClose(windows[1]))
    {
        int i;
        const vec3 colors[2] =
        {
            { 0.8f, 0.4f, 1.f },
            { 0.3f, 0.4f, 1.f }
        };

        for (i = 0;  i < 2;  i++)
        {
            int width, height;
            mat4x4 mvp;

            glfwGetFramebufferSize(windows[i], &width, &height);
            glfwMakeContextCurrent(windows[i]);

            GL_CHECK(glViewport(0, 0, width, height));

            mat4x4_ortho(mvp, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f);
			gl_program->setUniform(mvp_location, mvp);
			gl_program->setUniform(color_location, colors[i]);

#if 1
            GL_CHECK(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
#else
			std::array<uint16_t, 6> indices = { 0, 1, 2, 0, 2, 3 };
			GL_CHECK(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, indices.data()));
#endif

            glfwSwapBuffers(windows[i]);
        }

        glfwPollEvents();
    }

	std::static_pointer_cast<GLShader>(vertex_shader)->destroy(gl_program->getProgramID());
	std::static_pointer_cast<GLShader>(fragment_shader)->destroy(gl_program->getProgramID());

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

#if 0
{
	// Load the coordinate
	gl::VertexAttribPointer(m_aPositionHandle, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(FLOAT), vtxs);
	gl::VertexAttribPointer(m_aTexHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(FLOAT), &vtxs[3]);

	if (bTriangleFan)
		gl::DrawArrays(GL_TRIANGLE_FAN, 0, 4);
	else {
		std::array<uint16_t, 6> indices = { 0, 1, 2, 0, 2, 3 };
		gl::DrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, indices.data()); 
	}
}

#endif


ProfileBusyWait::ProfileBusyWait()
{
	GLuint query[2];
	GL_CHECK(glGenQueries(2, query));
	GL_CHECK(glQueryCounter(query[0], GL_TIMESTAMP));
}

ProfileBusyWait::~ProfileBusyWait()
{
}
