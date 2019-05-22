#include "mesh.h"
#include <meshoptimizer.h>
#include <objparser.h>

using namespace el;

float NegativeIndexHelper(float* src, int i, int sub)
{
    return i < 0 ? 0.f : src[i * 3 + sub];
}

bool el::LoadMesh(Geometry* geometry, const std::string& filename)
{
    if (!geometry)
        return false;

    ObjFile obj;
    if (!objParseFile(obj, filename.c_str()))
        return false;

    size_t index_count = obj.f_size / 3;
    std::vector<Vertex> vertices(index_count);

    for (size_t i = 0; i < index_count; i++)
    {
        Vertex& v = vertices[i];

        int vi = obj.f[i * 3 + 0];
        int vti = obj.f[i * 3 + 1];
        int vni = obj.f[i * 3 + 2];

        v.x = NegativeIndexHelper(obj.v, vi, 0);
        v.y = NegativeIndexHelper(obj.v, vi, 1);
        v.z = NegativeIndexHelper(obj.v, vi, 2);
        v.nx = NegativeIndexHelper(obj.vn, vni, 0);
        v.ny = NegativeIndexHelper(obj.vn, vni, 1);
        v.nz = NegativeIndexHelper(obj.vn, vni, 2);
        v.tu = NegativeIndexHelper(obj.vt, vti, 0);
        v.tv = NegativeIndexHelper(obj.vt, vti, 1);
    }

    std::vector<uint32_t> indices(index_count);
    std::vector<uint32_t> remap(index_count);
    size_t vertex_count = meshopt_generateVertexRemap(remap.data(), 0, index_count, 
            vertices.data(), index_count, sizeof(Vertex));

    vertices.resize(vertex_count);

    meshopt_remapVertexBuffer(vertices.data(), vertices.data(), index_count, sizeof(Vertex), remap.data());
    meshopt_remapIndexBuffer(indices.data(), 0, index_count, remap.data());

    meshopt_optimizeVertexCache(indices.data(), indices.data(), index_count, vertex_count);
    meshopt_optimizeVertexFetch(vertices.data(), indices.data(), index_count, 
            vertices.data(), vertex_count, sizeof(Vertex));

    Mesh mesh = {};
    mesh.vertexOffset = uint32_t(geometry->vertices.size());
    mesh.vertexCount = uint32_t(vertices.size());
    mesh.indexOffset = uint32_t(geometry->indices.size());
    mesh.indexCount = uint32_t(indices.size());

    geometry->meshes.push_back(std::move(mesh));

    geometry->vertices.insert(geometry->vertices.end(), 
                             std::make_move_iterator(vertices.begin()), 
                             std::make_move_iterator(vertices.end()));

    geometry->indices.insert(geometry->indices.end(),
                             std::make_move_iterator(indices.begin()), 
                             std::make_move_iterator(indices.end()));

    return true;
}
