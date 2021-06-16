#include "mesh.h"

#include "../external/tinyobj/tiny_obj_loader.h"

#include "core/logger.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include <glm/gtx/hash.hpp>
#include <unordered_map>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 uv;

    // TODO: add texture coordinates
    bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal && color == other.color && uv == other.uv;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((((hash<glm::vec3>()(vertex.position) ^
                (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
                (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.uv) << 1);
        }
    };
}

Mesh::Mesh()
{
	m_vertexArray.reset(VertexArray::create());
}

void Mesh::load(const std::string& filename)
{
    std::vector<Vertex> vertices;
    std::vector<u32> indices;

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(filename))
	{
		if (!reader.Error().empty()) {
			SGSERROR("%s", reader.Error().c_str());
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		SGSWARN("%s", reader.Warning().c_str());
	}

	auto& attrib    = reader.GetAttrib();
	auto& shapes    = reader.GetShapes();
	auto& materials = reader.GetMaterials();

    std::unordered_map<Vertex, u32> uniqueVertices{};

    for (const auto& shape : shapes) 
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};

            // Fill vertex positions
            vertex.position.x = attrib.vertices[3 * index.vertex_index + 0];
            vertex.position.y = attrib.vertices[3 * index.vertex_index + 1];
            vertex.position.z = attrib.vertices[3 * index.vertex_index + 2];

            // If there are normals, fill the vertices up
            if(index.normal_index >= 0)
            {
                vertex.normal.x = attrib.normals[3 * index.normal_index + 0];
                vertex.normal.y = attrib.normals[3 * index.normal_index + 1];
                vertex.normal.z = attrib.normals[3 * index.normal_index + 2];
            }

            if (index.texcoord_index >= 0)
            {
                vertex.uv.x = attrib.texcoords[2 * index.texcoord_index + 0];
                vertex.uv.y = attrib.texcoords[2 * index.texcoord_index + 1];
            }

            // Add colors based on position
            vertex.color.r = attrib.colors[3 * index.vertex_index + 0];
            vertex.color.g = attrib.colors[3 * index.vertex_index + 1];
            vertex.color.b = attrib.colors[3 * index.vertex_index + 2];

            // if vertex is not duplicated store and add
            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<u32>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }

    std::vector<f32> aux;
    for (auto& vert : vertices)
    {
        aux.push_back(vert.position.x);
        aux.push_back(vert.position.y);
        aux.push_back(vert.position.z);
        aux.push_back(vert.normal.x);
        aux.push_back(vert.normal.y);
        aux.push_back(vert.normal.z);
        aux.push_back(vert.color.x);
        aux.push_back(vert.color.y);
        aux.push_back(vert.color.z);
        aux.push_back(vert.uv.x);
        aux.push_back(vert.uv.y);
    }

    m_vertexBuffer.reset(VertexBuffer::create(aux.data(), static_cast<u32>(aux.size() * sizeof(f32))));
    VertexBufferLayout layout = { 
        {ShaderDataType::Float3, "a_position"},
        {ShaderDataType::Float3, "a_normal"},
        {ShaderDataType::Float3, "a_color"},
        {ShaderDataType::Float2, "a_uv"}
    };
    m_vertexBuffer->setLayout(layout);
    m_vertexArray->addVertexBuffer(m_vertexBuffer);

    m_indexBuffer.reset(IndexBuffer::create(indices.data(), static_cast<u32>(indices.size())));
    m_vertexArray->setIndexBuffer(m_indexBuffer);
}