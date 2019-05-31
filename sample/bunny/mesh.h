#pragma once

#include <cstdint>
#include <vector>
#include <linmath.h>
#include <el/graphics_types.h>
#include <el/graphics_input_layout.h>

namespace el {

    struct Vertex
    {
        float x, y, z;
        float nx, ny, nz;
        float tu, tv;
        static GraphicsInputAttributes getAttributeDescription()
        {
            GraphicsInputAttributes attrib(3);
            attrib[0] = GraphicsInputAttribute(0, "vPosition", 0, GraphicsVertexFormat::Float3, offsetof(Vertex, x));
            attrib[1] = GraphicsInputAttribute(0, "vNormal", 1, GraphicsVertexFormat::Float3, offsetof(Vertex, nx));
            attrib[2] = GraphicsInputAttribute(0, "vTexcoord", 2, GraphicsVertexFormat::Float2, offsetof(Vertex, tu));
            return attrib;
        }

        static GraphicsInputBindings getBindingDescription()
        {
            GraphicsInputBindings bindings(1);
            bindings[0] = GraphicsInputBinding(0, sizeof(Vertex));
            return bindings;
        }
    };

    struct Mesh
    {
        uint32_t vertexOffset;
        uint32_t vertexCount;
        uint32_t indexOffset;
        uint32_t indexCount;
    };

    struct MeshDraw
    {
        vec3 translate;
        float scale;
        quat orientation;
        uint32_t meshIndex;
    };

    struct Geometry
    {
        uint32_t getVertexCount() const { return vertices.size(); }
        uint32_t getIndexCount() const { return indices.size(); }
        const stream_t* getVertexData() const { return (stream_t*)vertices.data(); }
        const stream_t* getIndexData() const { return (stream_t*)indices.data(); }

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Mesh> meshes;
    };

    bool LoadMesh(Geometry* geometry, const std::string& filename);

} // namespace el {
