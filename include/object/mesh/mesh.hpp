#pragma once
#include "mesh_data.hpp"
#include <object/material/texture.hpp>

// Содержит массивы вершинных атрибутов
// Имеет материал связанный с шейдером и имеющий настроенные параметры
// В материалах подчастей модели указаны необходимые текстуры
class Mesh {
protected:
    using MeshEntry = MeshData::MeshEntry;

public:
    explicit Mesh(const MeshData& meshData, Shader* shader);
    void InitBuffers();
    void AddAttribute(const void* data, size_t size, const std::string& attrName, GLint sizePerVertex,
                   GLenum type = GL_FLOAT, GLboolean normalized = GL_FALSE,
                   GLsizei stride = 0, size_t offset = 0);
    void LinkUniforms();
    void Bind() const;
    void BindGeometry() const;
    Shader* GetShader() const;

    static Mesh* Create(const std::string& name, const MeshData& meshData, Shader* shader);
    static Mesh* Find(const std::string &name);
    static void Delete(const std::string &name);
    static void ClearСache();

    struct VertexBuffer {
        GLuint id;
        GLint location;
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        size_t offset;
    };

    GLuint vao = 0;
    GLuint ebo = 0;
    std::vector<GLuint> buffers;
    std::vector<VertexBuffer> vbos;
    std::vector<MeshEntry> m_Entries;
    Material material;
    Shader* shader = nullptr;
    
    inline static std::unordered_map<std::string, Mesh> cache;
};
