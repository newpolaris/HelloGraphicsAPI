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
#include <OpenGL/gl.h>

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

#include <graphics_device.h>
#include <graphics_context.h>
#include <graphics_texture.h>
#include <graphics_types.h>
#include <graphics_shader.h>
#include <graphics_program.h>
#include <graphics_texture.h>
#include <graphics_buffer.h>

#include <cstdio>

// TODO:
#include <OpenGL/gl_profile.h>

#include <stb/stb_image.h>
#include <utility.h>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

#if EL_CONFIG_DEBUG

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

    EL_TRACE("\n%s", output.str().c_str());
    el::debug_break();
}
#endif // EL_CONFIG_DEBUG

namespace el {

    class ImageData
    {
    public:

        uint32_t width;
        uint32_t height;
        uint32_t depth;
        std::vector<stream_t> stream;
        GraphicsPixelFormat format;
    };

    typedef std::shared_ptr<class ImageData> ImageDataPtr;

    ImageDataPtr ImageLoader(const std::string& filename)
    {
        stbi_set_flip_vertically_on_load(true);

        int width = 0, height = 0, components = 0;
        auto imagedata = (stream_t*)stbi_load(filename.c_str(), &width, &height, &components, 0);
        if (!imagedata) return nullptr;

        // 1-byte aligment image
        const streamsize_t length = width * height * components;

        GraphicsPixelFormat format = GraphicsPixelFormat::GraphicsPixelFormatInvalid;
        switch (components)
        {
        case 1: format = GraphicsPixelFormat::GraphicsPixelFormatR8Unorm; break;
        case 2: format = GraphicsPixelFormat::GraphicsPixelFormatRG8Unorm; break;
        case 3: format = GraphicsPixelFormat::GraphicsPixelFormatRGB8Unorm; break;
        case 4: format = GraphicsPixelFormat::GraphicsPixelFormatRGBA8Unorm; break;
        }

        auto container = std::make_shared<ImageData>();
        container->format = format;
        container->stream = std::vector<stream_t>(imagedata, imagedata + length);
        container->width = (int32_t)width;
        container->height = (int32_t)height;

        stbi_image_free(imagedata);

        return container;
    }
}

class GraphicsApplication
{
public:
};

int main(int argc, char** argv)
{
    GLFWwindow* windows[2];

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

#ifdef GLFW_INCLUDE_ES3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif 0 // COMPATIBILITY MODE
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#   if EL_PLAT_APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#   else
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#   endif
#else // LEGACY
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    windows[0] = glfwCreateWindow(400, 400, "First", NULL, NULL);
    if (!windows[0])
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(windows[0], key_callback);

    glfwMakeContextCurrent(windows[0]);

    // TODO:
    // how to handle glad_glGenFramebuffersEXT?
    // GL_IMPORT_EXT__(true, PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
    if (glGenFramebuffers != 0)
    {
        EL_TRACE("Require GL_ARB_framebuffer_object");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Only enable vsync for the first of the windows to be swapped to
    // avoid waiting out the interval for each window
    glfwSwapInterval(1);

    // The contexts are created with the same APIs so the function
    // pointers should be re-usable between them
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

#if EL_CONFIG_DEBUG
    if (glDebugMessageCallback) {
        GL_CHECK(glEnable(GL_DEBUG_OUTPUT));
        GL_CHECK(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
        GL_CHECK(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE));
        GL_CHECK(glDebugMessageCallback(gl_debug_callback, nullptr));
    }

    std::printf("%s\n%s\n%s\n%s\n",
        glGetString(GL_RENDERER),  // e.g. Intel HD Graphics 3000 OpenGL Engine
        glGetString(GL_VERSION),   // e.g. 3.2 INTEL-8.0.61
        glGetString(GL_VENDOR),    // e.g. NVIDIA Corporation
        glGetString(GL_SHADING_LANGUAGE_VERSION)  // e.g. 4.60 NVIDIA or 1.50 NVIDIA via Cg compiler
    );
#endif

    using namespace el;

    GraphicsDeviceDesc deviceDesc;
    deviceDesc.setType(GraphicsDeviceTypeOpenGL);

    GraphicsDevicePtr device = createDevice(deviceDesc);

    GraphicsShaderPtr vertex_shader;
    GraphicsShaderPtr fragment_shader;
    GraphicsProgramPtr program;
    GraphicsTexturePtr texture;

    GraphicsBufferPtr vertex_buffer, index_buffer;

    if (true)
    {
        const ImageDataPtr image = ImageLoader("miku.png");
        EL_ASSERT(image);

        GraphicsTextureDesc texture_desc;
        texture_desc.setDim(GraphicsTextureDim2D);
        texture_desc.setPixelFormat(image->format);
        texture_desc.setWidth(image->width);
        texture_desc.setHeight(image->height);
        texture_desc.setStream(static_cast<stream_t*>(image->stream.data()));
        texture_desc.setStreamSize(image->stream.size());
        texture_desc.setPixelAlignment(GraphicsPixelAlignment::GraphicsPixelAlignment1);

        texture = device->createTexture(texture_desc);
        EL_ASSERT(texture);
    }
    else
    {
        int x, y;
        char pixels[16 * 16];

        srand((unsigned int)glfwGetTimerValue());

        for (y = 0; y < 16; y++)
        {
            for (x = 0; x < 16; x++)
                pixels[y * 16 + x] = rand() % 256;
        }

        GraphicsTextureDesc texture_desc;
        texture_desc.setDim(GraphicsTextureDim2D);
        texture_desc.setPixelFormat(GraphicsPixelFormat::GraphicsPixelFormatR8Unorm);
        texture_desc.setWidth(16);
        texture_desc.setHeight(16);
        texture_desc.setStream(static_cast<stream_t*>(pixels));
        texture_desc.setStreamSize(16 * 16);

        texture = device->createTexture(texture_desc);
        EL_ASSERT(texture);
    }

    const vec2 vertices[4] =
    {
        {0.f, 0.f},
        {1.f, 0.f},
        {1.f, 1.f},
        {0.f, 1.f}
    };

    const char indices[] = { 0, 1, 2, 3 };

    // Create the OpenGL objects inside the first context, created above
    // All objects will be shared with the second context, created below
    {
        const auto vertex_shader_text = fileread("main.vert");
        EL_ASSERT(vertex_shader_text);

        GraphicsShaderDesc vertex_desc;
        vertex_desc.setStageFlag(GraphicsShaderStageVertexBit);
        vertex_desc.setShaderCode(vertex_shader_text.get());

        vertex_shader = device->createShader(vertex_desc);
        EL_ASSERT(vertex_shader);

        const auto frag_shader_text = fileread("main.frag");
        EL_ASSERT(frag_shader_text);

        GraphicsShaderDesc fragment_desc;
        fragment_desc.setStageFlag(GraphicsShaderStageFragmentBit);
        fragment_desc.setShaderCode(frag_shader_text.get());

        fragment_shader = device->createShader(fragment_desc);
        EL_ASSERT(fragment_shader);

        GraphicsProgramDesc program_desc;
        program_desc.addShader(vertex_shader);
        program_desc.addShader(fragment_shader);

        program = device->createProgram(program_desc);
        EL_ASSERT(program);

        GraphicsBufferDesc vertices_buffer_desc;
        vertices_buffer_desc.setDataType(GraphicsDataTypeStorageVertexBuffer);
        vertices_buffer_desc.setData((const char*)vertices);
        vertices_buffer_desc.setDataSize(sizeof(vertices));

        vertex_buffer = device->createBuffer(vertices_buffer_desc);

        GraphicsBufferDesc indices_buffer_desc;
        indices_buffer_desc.setDataType(GraphicsDataTypeStorageIndexBuffer);
        indices_buffer_desc.setData((const char*)indices);
        indices_buffer_desc.setDataSize(sizeof(indices));

        index_buffer = device->createBuffer(indices_buffer_desc);
    }

    GraphicsContextPtr context[2];
    context[0] = device->createDeviceContext();
    context[0]->setProgram(program); 
    context[0]->setTexture("texture", texture);
    context[0]->setVertexBuffer("vPos", vertex_buffer, sizeof(vertices[0]), 0);
    context[0]->setIndexBuffer(index_buffer);

    GLProfileBusyWait profile[2];
    profile[0].setName("window 0");
    profile[0].setDevice(device);
    profile[0].create();
    profile[1].setName("window 1");
    profile[1].setDevice(device);
    profile[1].create();

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
    context[1] = device->createDeviceContext();
    context[1]->setProgram(program);
    context[1]->setTexture("texture", texture);
    context[1]->setVertexBuffer("vPos", vertex_buffer, sizeof(vertices[0]), 0);

    mat4x4 mvp;
    mat4x4_ortho(mvp, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f);

    context[1]->setUniform("MVP", mvp);

    while (!glfwWindowShouldClose(windows[0]) &&
        !glfwWindowShouldClose(windows[1]))
    {
        const vec3 colors[2] =
        {
            {0.8f, 0.4f, 1.f},
            {0.3f, 0.4f, 1.f}
        };

        int i;
        for (i = 0; i < 2; i++)
        {
            int width, height;

            glfwGetFramebufferSize(windows[i], &width, &height);
            glfwMakeContextCurrent(windows[i]);

            profile[i].start();

            context[i]->beginRendering();
            context[i]->setProgram(program);
            context[i]->setViewport(Viewport(0, 0, width, height));

            // Shared bewteen context
            context[i]->setUniform("color", colors[i]);
            context[i]->draw(GraphicsPrimitiveType::GraphicsPrimitiveTypeFan, 0, 4);
            // context[i]->drawIndex(GraphicsPrimitiveType::GraphicsPrimitiveTypeFan, );

            profile[i].end();

            glfwSwapBuffers(windows[i]);
            char profileBuf[256] = {'\0'};
            sprintf(profileBuf, "%s CPU %.3f, GPU %.3f", profile[i].getName().c_str(),
                profile[i].getCpuTime(), profile[i].getGpuTime());
            glfwSetWindowTitle(windows[i], profileBuf);
        }

        glfwPollEvents();
    }

    vertex_shader.reset();
    fragment_shader.reset();
    program.reset();
    context[0].reset();
    context[1].reset();
    device.reset();

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
