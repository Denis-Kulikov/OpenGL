#pragma once

#include "lib-project/lib.hpp"
#include "math/Matrix4f.hpp"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


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
    void Render(Matrix4f<GLfloat>& mtx_transform);

#define INVALID_MATERIAL 0xFFFFFFFF
#define NUM_BONES_PER_VEREX 10

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

enum VB_TYPES {
    INDEX_BUFFER,
    POS_VB,
    NORMAL_VB,
    TEXCOORD_VB,
    BONE_VB,
    NUM_VBs            
};

    struct MeshEntry {
        MeshEntry();
        ~MeshEntry();
 
        void Init(const std::vector<Vertex>& Vertices,
                  const std::vector<unsigned int>& Indices);
 
        GLuint VB;
        GLuint IB;
        GLuint VAO;
        
        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
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

    struct VertexBoneData {
        void Mesh::VertexBoneData::AddBoneData(std::size_t BoneID, float Weight);
        static const int size = NUM_BONES_PER_VEREX;
        std::size_t IDs[size];
        float Weights[size];
    };

    struct BoneInfo {
        aiMatrix4x4 BoneOffset;
        aiMatrix4x4 FinalTransformation;

        BoneInfo() {
            BoneOffset = aiMatrix4x4();
            FinalTransformation = aiMatrix4x4();
        }
    };


private:
    void push_uniforms(Matrix4f<GLfloat>& mtx_transform);

    std::size_t FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    std::size_t FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    std::size_t FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);


    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);

    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform);
    void BoneTransform(float TimeInSeconds, std::vector<aiMatrix4x4>& Transforms);
    void LoadBones(std::size_t MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones);
    GLuint loadShader(const std::string &shaderPath, GLuint type);
    void compileShaders(const std::string &FS, const std::string &VS);
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    bool LoadEmbeddedTexture(const aiScene* scene, const std::string& fileName, int materialIndex);
    void InitMesh(std::size_t MeshIndex,
                    const aiMesh* paiMesh,
                    std::vector<glm::vec3>& Positions,
                    std::vector<glm::vec3>& Normals,
                    std::vector<glm::vec2>& TexCoords,
                    std::vector<VertexBoneData>& Bones,
                    std::vector<std::size_t>& Indices);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();
    
    Assimp::Importer Importer;
    const aiScene* m_pScene;
    aiMatrix4x4 m_GlobalInverseTransform;

    GLuint gBones;
    GLuint gProjection;
    GLuint gView;

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

    GLuint m_VAO;
    GLuint m_Buffers[NUM_VBs];

    std::vector<MeshEntry> m_Entries;
    std::vector<textures_names> m_Textures;
    std::size_t m_NumBones = 0;
    std::unordered_map<std::string, std::size_t> m_BoneMapping;
    std::vector<BoneInfo> m_BoneInfo;
    std::vector<VertexBoneData> Bones;
    std::vector<MaterialUniforms*> m_material;
};
