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
#include <graphics_input_layout.h>

#include <cstdio>
#include <cmath>
#include <random>

#include <utility.h>
#include <image.h>

#include "mesh.h"

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

namespace el {

class GraphicsApplication
{
public:

    GraphicsApplication();

    void run();

private:

    void initWindow();
    void initGraphics();
    void mainLoop();
    void cleanup();

};

GraphicsApplication::GraphicsApplication()
{
}

void GraphicsApplication::initWindow()
{
}

void GraphicsApplication::initGraphics()
{
}

void GraphicsApplication::mainLoop()
{
}

void GraphicsApplication::cleanup()
{
}

void GraphicsApplication::run()
{
    initWindow();
    initGraphics();
    mainLoop();
    cleanup();
}

} // namespace el {

namespace el {

    const std::string getResourcePath()
    {
#if EL_PLAT_IOS
        return[NSBundle.mainBundle.resourcePath stringByAppendingString : @"/data / "].UTF8String;
#elif EL_PLAT_ANDROID
        return "";
#else
        return RESOURCE_PATH;
#endif
    }

    float radians(float degrees)
    {
        const float pi = std::acos(-1.f);
        return degrees * pi / 180.f;
    }

}


int main(int argc, char** argv)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

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

    GLFWwindow* windows[2];
    windows[0] = glfwCreateWindow(1024, 768, "First", NULL, NULL);
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
    GraphicsDataPtr vertex_buffer;
    GraphicsDataPtr index_buffer;
    GraphicsInputLayoutPtr input_layout;

    std::vector<std::string> objfiles = {
        "kitten.obj",
        "rabbit.obj",
        "wolf.obj",
    };
    Geometry geometry;
    for (uint32_t i = 0; i < objfiles.size(); i++)
        EL_ASSERT(LoadMesh(&geometry, getResourcePath() + objfiles[i]));

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

        GraphicsDataDesc vertices_buffer_desc;
        vertices_buffer_desc.setDataType(GraphicsDataTypeStorageVertexBuffer);
        vertices_buffer_desc.setData(geometry.getVertexData());
        vertices_buffer_desc.setElementSize(sizeof(Vertex));
        vertices_buffer_desc.setNumElements(geometry.getVertexCount());

        vertex_buffer = device->createBuffer(vertices_buffer_desc);

        GraphicsDataDesc indices_buffer_desc;
        indices_buffer_desc.setDataType(GraphicsDataTypeStorageIndexBuffer);
        indices_buffer_desc.setData(geometry.getIndexData());
        indices_buffer_desc.setElementSize(sizeof(uint32_t));
        indices_buffer_desc.setNumElements(geometry.getIndexCount());

        index_buffer = device->createBuffer(indices_buffer_desc);

        GraphicsInputLayoutDesc input_layout_desc;
        input_layout_desc.setAttributes(Vertex::getAttributeDescription());
        input_layout_desc.setBindings(Vertex::getBindingDescription());
        input_layout = device->createInputLayout(input_layout_desc);
    }

    // TODO:
    // context[0]->setInputLayout(input_layout);
    // context[0]->setVertexBuffer(0, vertex_buffer);

    const float fNear = 0.1f;
    const float fFar = 1000.f;
    std::default_random_engine eng {10};
    std::uniform_real_distribution<float> urd(0, 1);
    const uint32_t draw_count = 2000;
    std::vector<MeshDraw> draws(draw_count);
    for (uint32_t i = 0; i < draw_count; i++) {
        vec3 axis;
        axis[0] = urd(eng)*2 - 1;
        axis[1] = urd(eng)*2 - 1;
        axis[2] = urd(eng)*2 - 1;

        const float angle = radians(urd(eng) * 90.f);

        draws[i].translate[0] = urd(eng) * 20.f - 10.f;
        draws[i].translate[1] = urd(eng) * 20.f - 10.f;
        draws[i].translate[2] = urd(eng) * -20.f - fNear;
        draws[i].scale = urd(eng) + 0.5f;
        quat_rotate(draws[i].orientation, angle, axis); 
        draws[i].meshIndex = static_cast<uint32_t>(urd(eng) * geometry.meshes.size());
    }

    GraphicsContextPtr context[2];
    context[0] = device->createDeviceContext();
    context[0]->setProgram(program);

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

    while (!glfwWindowShouldClose(windows[0]) 
        && !glfwWindowShouldClose(windows[1]))
    {
        int i;
        for (i = 0; i < 2; i++)
        {
            int width, height;

            glfwGetFramebufferSize(windows[i], &width, &height);
            glfwMakeContextCurrent(windows[i]);

            profile[i].start();

            context[i]->beginRendering();

            mat4x4 project;
            const float aspect = static_cast<float>(width) / height;
            mat4x4_perspective(project, radians(70.f), aspect, fNear, fFar);
            context[i]->setUniform("uProject", project);

            // TODO: setPipeline etc;
            context[i]->setViewport(Viewport(0, 0, width, height));
            context[i]->setIndexBuffer(index_buffer);
            for (auto& draw : draws)
            {
                const auto& mesh = geometry.meshes[draw.meshIndex];
                const uint32_t vertexOffsetInByte = mesh.vertexOffset * sizeof(Vertex);
                context[i]->setVertexBuffer("vPosition", vertex_buffer, sizeof(Vertex), offsetof(Vertex, x));
                context[i]->setVertexBuffer("vNormal", vertex_buffer, sizeof(Vertex), offsetof(Vertex, nx));
                context[i]->setVertexBuffer("vTexcoord", vertex_buffer, sizeof(Vertex), offsetof(Vertex, tu));

                context[i]->setUniform("uScale", draw.scale);
                context[i]->setUniform("uTranslate", draw.translate);
                context[i]->setUniform("uOrientation", draw.orientation);

                // Shared bewteen context
                context[i]->drawIndexed(GraphicsPrimitiveType::GraphicsPrimitiveTypeTriangle, mesh.indexCount, mesh.indexOffset, mesh.vertexOffset);
            }

            profile[i].end();
            context[i]->endRendering();

            glfwSwapBuffers(windows[i]);

            char profileBuf[256] = {'\0'};
            sprintf(profileBuf, "%s CPU %.3f ms, GPU %.3f ms", 
                    profile[i].getName().c_str(), profile[i].getCpuTime(), profile[i].getGpuTime());
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
