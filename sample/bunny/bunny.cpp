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
#include <graphics_input_layout.h>
#include "graphics_window.h"

#include <cstdio>
#include <cmath>
#include <random>

#include <el_utility.h>
#include <el_image.h>
#include <memory>

#include "mesh.h"

// TODO:
#include <OpenGL/gl_profile.h>

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

    GLFWwindow* _get(const GraphicsWindowPtr& ptr)
    {
        auto glfw = std::static_pointer_cast<GraphicsWindowGLFW>(ptr);
        if (glfw == nullptr)
            return nullptr;
        return glfw->_window;
    }

} // namespace el {

namespace el { 
namespace math {

    struct mat4
    {
        enum zero {};
        enum identity {};

        typedef vec4<float> row_type;
        typedef vec4<float> col_type;

        static const int numRows = 4;
        static const int numCols = 4;

        col_type _data[numRows];

        mat4();
        explicit mat4(zero);

        template <
            typename A, typename B, typename C, typename D,
            typename E, typename F, typename G, typename H,
            typename I, typename J, typename K, typename L,
            typename M, typename N, typename O, typename P>
        explicit mat4(A m00, B m01, C m02, D m03,
                      E m10, F m11, G m12, H m13,
                      I m20, J m21, K m22, L m23,
                      M m30, N m31, O m32, P m33);

        const row_type& operator[](size_t row) const
        {
            EL_ASSERT(row < numRows);
            return _data[row];
        }

        row_type& operator[](size_t row)
        {
            EL_ASSERT(row < numRows);
            return _data[row];
        }

    };

    mat4::mat4()
    {
    }

    mat4::mat4(zero)
    {
        for (int i = 0; i < 4; i++)
            _data[i] = row_type(0, 0, 0, 0);
    }

    template <
        typename A, typename B, typename C, typename D,
        typename E, typename F, typename G, typename H,
        typename I, typename J, typename K, typename L,
        typename M, typename N, typename O, typename P>
    mat4::mat4(A m00, B m01, C m02, D m03,
               E m10, F m11, G m12, H m13,
               I m20, J m21, K m22, L m23,
               M m30, N m31, O m32, P m33)
    {
        _data[0] = row_type(m00, m01, m02, m03);
        _data[1] = row_type(m10, m11, m12, m13);
        _data[2] = row_type(m20, m21, m22, m23);
        _data[3] = row_type(m30, m31, m32, m33);
    }

}
}

int bunny_run()
{
    using namespace el;

    GraphicsWindowPtr windows[2];
    GraphicsContextPtr context[2];

    GraphicsWindowDesc window_desc[2];
    window_desc[0].setWidth(1024);
    window_desc[0].setHeight(768);
    window_desc[0].setWindowTitle("First");
    window_desc[0].setDeviceType(GraphicsDeviceTypeOpenGL);
    window_desc[0].setWindowType(GraphicsWindowTypeGLFW);

    windows[0] = createWindow(window_desc[0]);
    if (!windows[0]) 
        return false;

    windows[0]->makeContextCurrent();

    GraphicsDeviceDesc deviceDesc;
    deviceDesc.setType(GraphicsDeviceTypeOpenGL);

    GraphicsDevicePtr device = createDevice(deviceDesc);
    context[0] = device->createContext();

    GraphicsShaderPtr vertex_shader;
    GraphicsShaderPtr fragment_shader;
    GraphicsProgramPtr program;
    GraphicsTexturePtr texture;
    GraphicsDataPtr vertex_buffer;
    GraphicsDataPtr index_buffer;
    GraphicsInputLayoutPtr input_layout;

    const std::string objfiles[] = {
		"kitten.obj",
		"rabbit.obj",
		"wolf.obj",
    };
    Geometry geometry;
    for (uint32_t i = 0; i < el::countof(objfiles); i++)
        EL_ASSERT(LoadMesh(&geometry, el::getResourcePath() + objfiles[i]));

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

    // TODO:
    // context[0]->setInputLayout(input_layout);
    // context[0]->setVertexBuffer(0, vertex_buffer);

    const float fNear = 0.1f;
    const float fFar = 1000.f;
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

    context[0]->setProgram(program);

    GLProfileBusyWait profile[2];
    profile[0].setName("window 0");
    profile[0].setDevice(device);
    profile[0].create();
    profile[1].setName("window 1");
    profile[1].setDevice(device);
    profile[1].create();

    window_desc[1].setWidth(400);
    window_desc[1].setHeight(400);
    window_desc[1].setWindowTitle("Second");
    window_desc[1].setDeviceType(GraphicsDeviceTypeOpenGL);
    window_desc[1].setWindowType(GraphicsWindowTypeGLFW);
    windows[1] = createWindow(window_desc[1]);
    if (!windows[1])
        return false;

    // Place the second window to the right of the first
    {
        int xpos, ypos, left, right, width;

        glfwGetWindowSize(_get(windows[0]), &width, NULL);
        glfwGetWindowFrameSize(_get(windows[0]), &left, NULL, &right, NULL);
        glfwGetWindowPos(_get(windows[0]), &xpos, &ypos);
        glfwSetWindowPos(_get(windows[1]), xpos + width + left + right, ypos);
    }

    windows[1]->makeContextCurrent();

    // While objects are shared, the global context state is not and will
    // need to be set up for each context
    // context[1] = device->createDeviceContext();
    // context[1]->setProgram(program);

    while (!glfwWindowShouldClose(_get(windows[0]))
        && !glfwWindowShouldClose(_get(windows[1])))
    {
        int i;
        for (i = 0; i < 1; i++)
        {
            windows[i]->makeContextCurrent();

            auto width = windows[i]->getWidth();
            auto height = windows[i]->getHeight();

            profile[i].start();

            context[i]->beginRendering();

            // TODO:
            glClearColor(0.5f, 0.5f, 0.5f, 1.f);
            // TODO:
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mat4x4 project;
            const float aspect = static_cast<float>(width) / height;
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

            profile[i].end();
            context[i]->endRendering();

            glfwSwapBuffers(_get(windows[i]));

            char profileBuf[256] = {'\0'};
            sprintf(profileBuf, "%s CPU %.3f ms, GPU %.3f ms", 
                    profile[i].getName().c_str(), profile[i].getCpuTime(), profile[i].getGpuTime());

            glfwSetWindowTitle(_get(windows[i]), profileBuf);
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
    windows[1].reset();
    windows[0].reset();

    glfwTerminate();

    return true;
}

int main(int argc, char** argv)
{
    EL_ASSERT(bunny_run());
    return 0;
}
