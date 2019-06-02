#include <Windows.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <array>
#include <chrono>

#include <cstdio>
#include <cmath>
#include <random>
#include <memory>

#include <glad/glad.h>
#include <el/debug.h>

#include <el/graphics_device.h>
#include <el/graphics_context.h>
#include <el/graphics_texture.h>
#include <el/graphics_types.h>
#include <el/graphics_shader.h>
#include <el/graphics_program.h>
#include <el/graphics_texture.h>
#include <el/graphics_data.h>
#include <el/graphics_input_layout.h>
#include <el/utility.h>
#include <el/image.h>

#include <linmath.h>
#include <mesh.h>
#include <native_window_helper.h>

#include <OpenGL/gl_profile.h>

#pragma comment(lib, "Opengl32.lib")

namespace el {

    const char* vertexShader = R"""(
#version 110

attribute vec3 vPosition;
attribute vec3 vNormal;
attribute vec2 vTexcoord;

varying vec3 color;

uniform float uScale;
uniform vec3 uTranslate;
uniform vec4 uOrientation;
uniform mat4 uProject;

vec3 rotate_position(vec4 quat, vec3 v)
{
    return v + 2.0 * cross(quat.xyz, cross(quat.xyz, v) + quat.w * v);
}

void main()
{
    color = vNormal * 0.5 + vec3(0.5);
    gl_Position = uProject * vec4(rotate_position(uOrientation, vPosition) * uScale + uTranslate, 1.0);
}
)""";

    const char* fragmentShader = R"""(
#version 110

varying vec3 color;
void main()
{
    gl_FragColor = vec4(color, 1.0);
}
    )""";


    float radians(float degrees)
    {
        const float pi = std::acos(-1.f);
        return degrees * pi / 180.f;
    }

    /*
    const std::string getResourcePath()
    {
    #if EL_PLAT_IOS
        return [NSBundle.mainBundle.resourcePath stringByAppendingString : @"/data / "].UTF8String;
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
    */
} // namespace el

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002

typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC,HGLRC,const int*);

namespace el {

struct ContextRecovery
{
    ContextRecovery();
    ~ContextRecovery();

    HDC _dc;
    HGLRC _context;
};

ContextRecovery::ContextRecovery()
{
    _dc = wglGetCurrentDC();
    _context = wglGetCurrentContext();
}

ContextRecovery::~ContextRecovery()
{
    wglMakeCurrent(_dc, _context);
}

struct Swapchain
{
    bool create(void* nativeWindow, void* nativeSwapchain);
    void destroy();

    void makeCurrent();
    void swapbuffer();

    HDC _hdc;
    HWND _hwnd;
    HGLRC _context;
};

void Swapchain::destroy()
{
    wglMakeCurrent(NULL, NULL);
    _context = NULL;
    if (_hdc) {
        ReleaseDC(_hwnd, _hdc);
        _hdc = NULL;
    }
    _hwnd = NULL;
}

bool Swapchain::create(void* nativeWindow, void* nativeSwapchain)
{
    ContextRecovery _;

    _hwnd = (HWND)nativeWindow;
    _hdc = (HDC)nativeSwapchain;
    HGLRC dummyContext = wglCreateContext(_hdc);
    if (!dummyContext)
        return false;

    if (!wglMakeCurrent(_hdc, dummyContext))
    {
        wglDeleteContext(dummyContext);
        return false;
    }

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (wglCreateContextAttribs)
    {
        int attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 1,
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
            0, 0
        };

        _context = wglCreateContextAttribs(_hdc, nullptr, attribs);
    }
    else
    {
        _context = wglCreateContext(_hdc);
    }

    wglMakeCurrent(_hdc, _context);
    gladLoadGL();
    return true;
}

void Swapchain::makeCurrent()
{
    wglMakeCurrent(_hdc, _context);
}

void Swapchain::swapbuffer()
{
    SwapBuffers(_hdc);
}

struct App
{
    virtual bool setup();
    virtual void cleanup();

    void activate();
    void swapbuffer();
    void run();

    virtual void render() = 0;

    Swapchain _swapchain;
    SDL_Window* _window;
};

bool App::setup()
{
    SDL_Init(SDL_INIT_VIDEO);

    _window = SDL_CreateWindow("bunny-sample",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            1280, 768,
            SDL_WINDOW_RESIZABLE);

    if (_window == nullptr)
        return false;

    void* nativeWindow = ::getNativeWindow(_window);
    if (!_swapchain.create(nativeWindow, nativeWindow))
        return false;

    return true;
}

void App::cleanup()
{
    _swapchain.destroy();

    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void App::activate()
{
    _swapchain.makeCurrent();
}

void App::swapbuffer()
{
    _swapchain.swapbuffer();
}

void App::run()
{
    while (true)
    {
        bool isQuit = false;
        SDL_Event e;
        while (SDL_PollEvent(&e))
            isQuit = (e.type == SDL_QUIT);
        if (isQuit) break;

        activate();
        render();
        swapbuffer();
    }
}

struct BunnyApp : public App
{
    const float fNear = 0.1f;
    const float fFar = 1000.f;

    GraphicsShaderPtr vertex_shader;
    GraphicsShaderPtr fragment_shader;
    GraphicsProgramPtr program;
    GraphicsTexturePtr texture;
    GraphicsDataPtr vertex_buffer;
    GraphicsDataPtr index_buffer;
    GraphicsInputLayoutPtr input_layout;

    bool setup() override;
    virtual void render() override;
};

bool BunnyApp::setup()
{
    if (!App::setup()) 
        return false;

    const std::string objfiles[] = {
		"kitten.obj",
		"rabbit.obj",
		"wolf.obj",
    };
    Geometry geometry;
    for (uint32_t i = 0; i < el::countof(objfiles); i++)
        EL_ASSERT(LoadMesh(&geometry, objfiles[i]));

#if 0
    // Create the OpenGL objects inside the first context, created above
    // All objects will be shared with the second context, created below
    {
        // const auto vertex_shader_text = fileread("main.vert");
        const auto vertex_shader_text = vertexShader;
        EL_ASSERT(vertex_shader_text);

        GraphicsShaderDesc vertex_desc;
        vertex_desc.setStageFlag(GraphicsShaderStageVertexBit);
        vertex_desc.setShaderCode(vertex_shader_text);

        vertex_shader = device->createShader(vertex_desc);
        EL_ASSERT(vertex_shader);

        // const auto frag_shader_text = fileread("main.frag");
        const auto frag_shader_text = fragmentShader;
        EL_ASSERT(frag_shader_text);

        GraphicsShaderDesc fragment_desc;
        fragment_desc.setStageFlag(GraphicsShaderStageFragmentBit);
        fragment_desc.setShaderCode(frag_shader_text);

        fragment_shader = device->createShader(fragment_desc);
        EL_ASSERT(fragment_shader);

        GraphicsProgramDesc program_desc;
        program_desc.addShader(vertex_shader);
        program_desc.addShader(fragment_shader);

        program = device->createProgram(program_desc);
        EL_ASSERT(program);

        GraphicsDataDesc vertices_buffer_desc;
        vertices_buffer_desc.setDataType(GraphicsDataTypeStorageVertexBuffer);
        vertices_buffer_desc.setStream(geometry.getVertexData());
        vertices_buffer_desc.setElementSize(sizeof(Vertex));
        vertices_buffer_desc.setNumElements(geometry.getVertexCount());

        vertex_buffer = device->createBuffer(vertices_buffer_desc);

        GraphicsDataDesc indices_buffer_desc;
        indices_buffer_desc.setDataType(GraphicsDataTypeStorageIndexBuffer);
        indices_buffer_desc.setStream(geometry.getIndexData());
        indices_buffer_desc.setElementSize(sizeof(uint32_t));
        indices_buffer_desc.setNumElements(geometry.getIndexCount());

        index_buffer = device->createBuffer(indices_buffer_desc);

        GraphicsInputLayoutDesc input_layout_desc;
        input_layout_desc.setAttributes(Vertex::getAttributeDescription());
        input_layout_desc.setBindings(Vertex::getBindingDescription());
        input_layout = device->createInputLayout(input_layout_desc);
    }

    std::default_random_engine eng(10);
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
#endif
    return true;
}

void BunnyApp::render()
{
#if 0
    int w, h;
    SDL_GetWindowSize(_window, &w, &h);

    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4x4 project;
    const float aspect = static_cast<float>(w) / h;
    mat4x4_perspective(project, radians(70.f), aspect, fNear, fFar);
    context[i]->setUniform("uProject", project);
    context[i]->setViewport(Viewport(0, 0, width, height));
    context[i]->setIndexBuffer(index_buffer);
    context[i]->setVertexBuffer("vPosition", vertex_buffer, sizeof(Vertex), offsetof(Vertex, x));
    context[i]->setVertexBuffer("vNormal", vertex_buffer, sizeof(Vertex), offsetof(Vertex, nx));
    context[i]->setVertexBuffer("vTexcoord", vertex_buffer, sizeof(Vertex), offsetof(Vertex, tu));
    for (auto& draw : draws)
    {
        const auto& mesh = geometry.meshes[draw.meshIndex];
        const uint32_t vertexOffsetInByte = mesh.vertexOffset * sizeof(Vertex);

        context[i]->setUniform("uScale", draw.scale);
        context[i]->setUniform("uTranslate", draw.translate);
        context[i]->setUniform("uOrientation", draw.orientation);

        // Shared bewteen context
        context[i]->drawIndexed(GraphicsPrimitiveTypeTriangle,
                                mesh.indexCount,
                                mesh.indexOffset,
                                mesh.vertexOffset);
    }
    context[i]->endRendering();
#endif
}

} // namespace el

int main(int argc, char** argv)
{
    el::BunnyApp app;

    EL_ASSERT(app.setup());
    app.run();

    return 0;
}
