#pragma once

#include "lib-project/lib.hpp"
#include "math/Matrix4f.hpp"
#include <assimp/scene.h>

struct Texture {
    Texture(const std::string& Filename);

    GLuint texture;
};

struct Vertex {
    Vertex() {}
    Vertex(const glm::vec3& pos, const glm::vec3& normal, const glm::vec3& Tangent, const glm::vec2& tex) 
        : m_pos(pos), m_tex(tex), m_normal(normal), m_tangent(Tangent)
    {}

    glm::vec3 m_pos;
    glm::vec3 m_normal;
    glm::vec3 m_tangent;
    glm::vec2 m_tex;
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
    GLuint gEyeWorldPosLocation;
    GLuint gTextureSamplerLocation;
    GLuint gNormalMapLocation;
    GLuint gWorldLocation;
    GLuint gColorLocation;
    GLuint gDirectionLocation;
    GLuint gAmbientIntensityLocation;
    GLuint gDiffuseIntensityLocation;
    GLuint gMatSpecularIntensityLocation;
    GLuint gSpecularPowerLocation;
    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Position;
        struct
        {
            GLuint Constant;
            GLuint Linear;
            GLuint Exp;
        } Atten;
    } gPointLightsLocation[1];

};
