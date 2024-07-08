#pragma once

#include "lib-project/lib.hpp"
#include "math/Matrix4f.hpp"
#include <assimp/scene.h>

struct Texture {
    Texture(const std::string& Filename);

    GLuint texture;
};

struct Vertex {
    glm::vec3 m_pos;
    glm::vec3 m_normal;
    glm::vec2 m_tex;

    Vertex() {}

    Vertex(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& tex)
    {
        m_pos    = pos;
        m_normal = normal;
        m_tex    = tex;
    }
};

class Mesh
{
public:
    Mesh(const std::string& Filename);
    ~Mesh();
 
    bool LoadMesh(const std::string& Filename);
    void Render(Matrix4f<GLfloat>& mtx_transform);
 
private:
    GLuint loadShader(const std::string &shaderPath, GLuint type);
    void compileShaders(const std::string &FS, const std::string &VS);
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();
 
#define INVALID_MATERIAL 0xFFFFFFFF
 
    struct MeshEntry {
        MeshEntry();
        ~MeshEntry();
 
        void Init(const std::vector<Vertex>& Vertices,
                  const std::vector<unsigned int>& Indices);
 
        GLuint VB;
        GLuint IB;
        GLuint VAO;

        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };
 
    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;

    GLuint shaderProgram;
    GLuint gWorldLocation;
    GLuint gTextureSamplerLocation;
};
