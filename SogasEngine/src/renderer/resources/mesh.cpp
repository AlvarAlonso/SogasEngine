#include "sgspch.h"

#include "mesh.h"
#include "renderer/shader.h"
#include "core/utils.h"
#include "core/logger.h"
#include "scene/components/cameraComponent.h"

#include "../external/tinyobj/tiny_obj_loader.h"
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace Sogas 
{

    extern std::vector<std::string> g_assetsPath;

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;
        glm::vec2 uv;

        bool operator==(const Vertex& other) const {
            return position == other.position && normal == other.normal && color == other.color && uv == other.uv;
        }
    };
}

namespace std {
    template<> struct hash<Sogas::Vertex> {
        size_t operator()(Sogas::Vertex const& vertex) const {
            return ((((hash<glm::vec3>()(vertex.position) ^
                (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
                (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.uv) << 1);
        }
    };
}

namespace Sogas 
{
    std::unordered_map<std::string, std::shared_ptr<Mesh>> Mesh::s_loadedMeshes;

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

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        MaterialProperties props{};
        if (!materials.empty())
        {
            auto& material = materials[0];
            props.color = glm::vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1.0f);
            props.metallicFactor = material.metallic;
            props.roughnessFactor = material.shininess;
        }

        m_pMaterial = std::make_shared<Material>(Shader::GET("phong.shader"), props);

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
                if (index.normal_index >= 0)
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
                    //vertex.entityId = entityId;
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }

        size_t arraySize = vertices.size();
        size_t vertexSize = sizeof(Vertex);
        size_t size = arraySize * vertexSize;

        m_vertexBuffer.reset(VertexBuffer::create(vertices.data(), (u32)size));
        VertexBufferLayout layout = {
            {ShaderDataType::Float3, "a_position"},
            {ShaderDataType::Float3, "a_normal"},
            {ShaderDataType::Float3, "a_color"},
            {ShaderDataType::Float2, "a_uv"}
        };
        m_vertexBuffer->setLayout(layout);
        m_vertexArray->addVertexBuffer(m_vertexBuffer);

        m_indexBuffer.reset(IndexBuffer::create(indices.data(), (u32)indices.size()));
        m_vertexArray->setIndexBuffer(m_indexBuffer);
    }

    /*
    * @brief This functions checks if the mesh is in the assets path and after being found, checks if it has been loaded.
    * If already loaded, returns the found one. Loads the mesh otherwise.
    */
    std::shared_ptr<Mesh> Mesh::GET(const std::string& filename)
    {
        if (!s_loadedMeshes[filename])
        {
            std::string path = findFile(filename, g_assetsPath);
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
            mesh->load(path);
            mesh->m_filename = filename;
            s_loadedMeshes[filename] = mesh;
            return mesh;
        }

        return s_loadedMeshes[filename];
    }

    void Mesh::createGrid(f32 dist, i32 n_lines)
    {
        glm::vec4 color = { 0.5f, 0.5f, 0.5f, 1.0f };

        struct gVertex {
            glm::vec3 position;
            glm::vec4 color;
        };

        // Vertex Buffer

        std::vector<gVertex> vertices;

        for (f32 i = n_lines * -0.5f; i <= n_lines * 0.5f; ++i)
        {
            glm::vec4 color = i32(i) % 10 == 0 ? glm::vec4(1.0f) : glm::vec4(0.75f);
            vertices.push_back({ {i * dist, 0.0f, dist * n_lines * -0.5f}, color });
            vertices.push_back({ {i * dist, 0.0f, dist * n_lines * +0.5f}, color });
            vertices.push_back({ {dist * n_lines * +0.5f, 0.0f, dist * i}, color });
            vertices.push_back({ {dist * n_lines * -0.5f, 0.0f, dist * i}, color });
        }

        size_t arraySize = vertices.size();
        size_t vertexSize = sizeof(gVertex);
        size_t size = arraySize * vertexSize;

        m_vertexBuffer.reset(VertexBuffer::create(vertices.data(), (u32)size));
        VertexBufferLayout layout = {
            {ShaderDataType::Float3, "a_position"},
            {ShaderDataType::Float4, "a_color"}
        };
        m_vertexBuffer->setLayout(layout);
        m_vertexArray->addVertexBuffer(m_vertexBuffer);
    }

    /*
    * @brief This function creates a simple wireframe mesh that represents the camera.
    * @param void
    * @return void
    */
    void Mesh::createCameraMesh(std::weak_ptr<CameraComponent>& cameraComponent)
    {
        glm::vec3 black = {0.0f, 0.0f, 0.0f};

        struct gVertex {
            glm::vec3 position;
            glm::vec3 color;
        };

        // Vertex Buffer
        std::vector<gVertex> vertices;

        CameraComponent::eProjectionType type = cameraComponent.lock()->getType();
        float aspectRatio = cameraComponent.lock()->getAspectRatio();

        if (type == CameraComponent::eProjectionType::Orthographic)
        {
            float size = cameraComponent.lock()->getOrthographicSize();
            float near = cameraComponent.lock()->getOrthographicNear();

            vertices.push_back({ { 0.0f,  0.0f,  0.0f}, {0.75f, 0.25f, 0.0f} });
            vertices.push_back({ { size * aspectRatio,  size,  near}, black });
            vertices.push_back({ { size * aspectRatio, -size,  near}, black });
            vertices.push_back({ {-size * aspectRatio,  size,  near}, black });
            vertices.push_back({ {-size * aspectRatio, -size,  near}, black });
        }
        else if (type == CameraComponent::eProjectionType::Perspective)
        {
            float FOV = cameraComponent.lock()->getPerspectiveFOV();
            float near = cameraComponent.lock()->getPerspectiveNear();
            float size = near * std::tan(FOV);

            vertices.push_back({ { 0.0f,  0.0f,  0.0f}, {0.75f, 0.25f, 0.0f} });
            vertices.push_back({ { size * aspectRatio,  size,  near}, black });
            vertices.push_back({ { size * aspectRatio, -size,  near}, black });
            vertices.push_back({ {-size * aspectRatio,  size,  near}, black });
            vertices.push_back({ {-size * aspectRatio, -size,  near}, black });
        }

        size_t arraySize    = vertices.size();
        size_t vertexSize   = sizeof(gVertex);
        size_t size         = arraySize * vertexSize;

        m_vertexBuffer.reset(VertexBuffer::create(vertices.data(), (u32)size));
        VertexBufferLayout layout = {
            {ShaderDataType::Float3, "a_position"},
            {ShaderDataType::Float3, "a_color"}
        };

        m_vertexBuffer->setLayout(layout);
        m_vertexArray->addVertexBuffer(m_vertexBuffer);

        // Indices Buffer
        std::vector<u32> indices = { 0, 1, 2, 0, 1, 3, 0, 3, 4, 0, 4, 2 };
        m_indexBuffer.reset(IndexBuffer::create(indices.data(), indices.size()));

        m_vertexArray->setIndexBuffer(m_indexBuffer);
    }

    void Mesh::getFrustrumFromCamera(std::weak_ptr<CameraComponent>& cameraComponent)
    {
        glm::vec3 black = { 0.0f, 0.0f, 0.0f };

        struct gVertex {
            glm::vec3 position;
            glm::vec3 color;
        };

        std::vector<gVertex> vertices;
        std::vector<u32> indices;
        float aspectRatio = cameraComponent.lock()->getAspectRatio();

        if (cameraComponent.lock()->getType() == CameraComponent::eProjectionType::Orthographic)
        {
            float size  = cameraComponent.lock()->getOrthographicSize();
            float near  = cameraComponent.lock()->getOrthographicNear();
            float far   = cameraComponent.lock()->getOrthographicFar();

            vertices.push_back({ { size * aspectRatio,  size,  near}, black });
            vertices.push_back({ { size * aspectRatio, -size,  near}, black });
            vertices.push_back({ {-size * aspectRatio,  size,  near}, black });
            vertices.push_back({ {-size * aspectRatio, -size,  near}, black });
                                                     
            vertices.push_back({ { size * aspectRatio,  size,  far}, black });
            vertices.push_back({ { size * aspectRatio, -size,  far}, black });
            vertices.push_back({ {-size * aspectRatio,  size,  far}, black });
            vertices.push_back({ {-size * aspectRatio, -size,  far}, black });

        }

        if (cameraComponent.lock()->getType() == CameraComponent::eProjectionType::Perspective)
        {
            float fov   = cameraComponent.lock()->getPerspectiveFOV();
            float near  = cameraComponent.lock()->getPerspectiveNear();
            float far   = cameraComponent.lock()->getPerspectiveFar();
            float size  = 1.0f;

            vertices.push_back({ { size * aspectRatio,  size,  near}, black });
            vertices.push_back({ { size * aspectRatio, -size,  near}, black });
            vertices.push_back({ {-size * aspectRatio,  size,  near}, black });
            vertices.push_back({ {-size * aspectRatio, -size,  near}, black });

            vertices.push_back({ { size * aspectRatio,  size,  far}, black });
            vertices.push_back({ { size * aspectRatio, -size,  far}, black });
            vertices.push_back({ {-size * aspectRatio,  size,  far}, black });
            vertices.push_back({ {-size * aspectRatio, -size,  far}, black });
        }

        size_t arraySize = vertices.size();
        size_t vertexSize = sizeof(gVertex);
        size_t size = arraySize * vertexSize;

        m_vertexBuffer.reset(VertexBuffer::create(vertices.data(), (u32)size));
        VertexBufferLayout layout = {
            {ShaderDataType::Float3, "a_position"},
            {ShaderDataType::Float3, "a_color"}
        };

        m_vertexBuffer->setLayout(layout);
        m_vertexArray->addVertexBuffer(m_vertexBuffer);

        indices = {0, 4, 4, 5, 5, 1, 0, 4, 4, 6, 6, 2, 3, 7, 7, 6, 7, 5};
        m_indexBuffer.reset(IndexBuffer::create(indices.data(), indices.size()));

        m_vertexArray->setIndexBuffer(m_indexBuffer);
    }
}