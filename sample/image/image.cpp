#include <el_predefine.h>
#include <el_debug.h>

#include <OpenGL/gl_headers.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <array>
#include <chrono>

#include <getopt.h>
#include <linmath.h>

#include <graphics_device.h>
#include <graphics_context.h>
#include <graphics_texture.h>
#include <graphics_types.h>
#include <graphics_shader.h>
#include <graphics_program.h>
#include <graphics_texture.h>
#include <graphics_data.h>

#include <cstdio>

#include <el_utility.h>
#include <el_image.h>
#include <objparser.h>
#include <meshoptimizer.h>

// TODO:
#include <OpenGL/gl_profile.h>

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

class GraphicsApplication
{
public:
};

namespace el {

    const std::string getResourcePath()
    {
#if EL_PLAT_IOS
        return[NSBundle.mainBundle.resourcePath stringByAppendingString : @"/data / "].UTF8String;
#elif EL_PLAT_ANDROID
        return "";
#else
        return EL_DEFINE_RESOURCE_PATH;
#endif
    }
    
    const std::string getSamplePath()
    {
        return EL_DEFINE_SAMPLE_PATH;
    }
}

int main(int argc, char** argv)
{
    GLFWwindow* windows[2];

#ifdef GLFW_INCLUDE_ES3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif 1 // COMPATIBILITY MODE
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

    // Only enable vsync for the first of the windows to be swapped to
    // avoid waiting out the interval for each window
    glfwSwapInterval(1);

    // The contexts are created with the same APIs so the function
    // pointers should be re-usable between them
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // TODO:
    // how to handle glad_glGenFramebuffersEXT?
    // GL_IMPORT_EXT__(true, PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
    if (glGenFramebuffers == 0)
    {
        EL_TRACE("Require GL_ARB_framebuffer_object");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

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

    GraphicsDataPtr vertex_buffer, index_buffer;

	const ImageDataPtr image = ImageData::load(getResourcePath() + "miku.png");
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

    const vec2 vertices[4] =
    {
        {0.f, 0.f},
        {1.f, 0.f},
        {1.f, 1.f},
        {0.f, 1.f}
    };

    const uint32_t indices[] = { 0, 1, 2, 3 };

    // Create the OpenGL objects inside the first context, created above
    // All objects will be shared with the second context, created below
    {
        const auto vertex_shader_text = fileread(getSamplePath() + "main.vert");
        EL_ASSERT(vertex_shader_text);
        
        GraphicsShaderDesc vertex_desc;
        vertex_desc.setStageFlag(GraphicsShaderStageVertexBit);
        vertex_desc.setShaderCode(vertex_shader_text.get());

        vertex_shader = device->createShader(vertex_desc);
        EL_ASSERT(vertex_shader);

        const auto frag_shader_text = fileread(getSamplePath() + "main.frag");
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

        GraphicsDataDesc vertices_buffer_desc;
        vertices_buffer_desc.setDataType(GraphicsDataTypeStorageVertexBuffer);
        vertices_buffer_desc.setStream((const stream_t*)vertices);
        vertices_buffer_desc.setElementSize(sizeof(vec2));
        vertices_buffer_desc.setNumElements(countof(vertices));

        vertex_buffer = device->createBuffer(vertices_buffer_desc);

        GraphicsDataDesc indices_buffer_desc;
        indices_buffer_desc.setDataType(GraphicsDataTypeStorageIndexBuffer);
        indices_buffer_desc.setStream((const char*)indices);
        indices_buffer_desc.setElementSize(sizeof(uint32_t));
        indices_buffer_desc.setNumElements(countof(indices));

        index_buffer = device->createBuffer(indices_buffer_desc);
    }

    GraphicsContextPtr context[2];
    context[0] = device->createContext();
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
    context[1] = device->createContext();
    context[1]->setProgram(program);
    context[1]->setTexture("texture", texture);
    context[1]->setVertexBuffer("vPos", vertex_buffer, sizeof(vertices[0]), 0);
    context[1]->setIndexBuffer(index_buffer);

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

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);

            context[i]->beginRendering();
            context[i]->setProgram(program);
            context[i]->setViewport(Viewport(0, 0, width, height));

            // Shared bewteen context
            context[i]->setUniform("color", colors[i]);
            context[i]->drawIndexed(GraphicsPrimitiveType::GraphicsPrimitiveTypeFan, 4);

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
    vertex_buffer.reset();
    index_buffer.reset();
    context[0].reset();
    context[1].reset();
    device.reset();

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
