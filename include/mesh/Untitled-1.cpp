#pragma once

#include "lib-project/lib.hpp"
#include "math/Matrix4f.hpp"
#include <assimp/scene.h>

typedef struct stb_img_struct {
    stb_img_struct(const std::string& Filename);
    ~stb_img_struct();

    int x, y, n;
    unsigned char *img = nullptr;
} stb_img;

struct Texture {
    Texture(const stb_img& Filename);

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
    void Render(const objectTransform &model_transform);
 
private:
    GLuint loadShader(const std::string &shaderPath, GLuint type);
    void compileShaders(const std::string &FS, const std::string &VS);
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    bool LoadEmbeddedTexture(const aiScene* scene, const std::string& fileName, int materialIndex);
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

    struct textures_names {
        textures_names() 
            : dif(nullptr), nrm(nullptr), spc(nullptr), lmp(nullptr) {};
        textures_names(Texture *dif_, Texture *nrm_, Texture *spc_, Texture *lmp_)
            : dif(dif_), nrm(nrm_), spc(spc_), lmp(lmp_) {};
        Texture *dif;
        Texture *nrm;
        Texture *spc;
        Texture *lmp;
    };
 
    struct MaterialUniforms {
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
    };

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

    GLuint gModelLocation;
    GLuint gViewLocation;
    GLuint gProjectionLocation;

    GLuint gDirLightColorLocation;
    GLuint gDirLightDirectionLocation;
    GLuint gDirLightAmbientIntensityLocation;
    GLuint gDirLightDiffuseIntensityLocation;

    GLuint gPointLightPosLocation;
    GLuint gPointLightColorLocation;
    GLuint gPointLightAmbientIntensityLocation;
    GLuint gPointLightDiffuseIntensityLocation;
    GLuint gPointLightConstantLocation;
    GLuint gPointLightLinearLocation;
    GLuint gPointLightExpLocation;

    std::vector<MeshEntry> m_Entries;
    std::vector<textures_names> m_Textures;
    std::vector<MaterialUniforms*> m_material;
};
