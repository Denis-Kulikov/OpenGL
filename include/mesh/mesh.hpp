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
    Texture(const stb_img& img, const std::string &n);
    Texture(const stb_img& img);

    std::string name;
    GLuint texture;
};


class Mesh
{
public:
    Mesh(const std::string& Filename);
    ~Mesh();
 
    void set_transform(const objectTransform &transform);
    bool LoadMesh(const std::string& Filename);
    void Render(objectTransform &trans);

#define INVALID_MATERIAL 0xFFFFFFFF
#define NUM_BONES_PER_VEREX 4

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   2
#define NORMAL_LOCATION      1
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
        void AddBoneData(unsigned int BoneID, float Weight);
        void normalize();
        static const int size = NUM_BONES_PER_VEREX;
        unsigned int IDs[size];
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
    void push_uniforms(objectTransform &trans);

    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);


    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);

    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform);
    void BoneTransform(float TimeInSeconds, std::vector<aiMatrix4x4>& Transforms);
    
    void LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones);
    GLuint loadShader(const std::string &shaderPath, GLuint type);
    void compileShaders(const std::string &FS, const std::string &VS);
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    bool LoadEmbeddedTexture(const aiScene* scene, const std::string& fileName, int materialIndex);
    void InitMesh(unsigned int MeshIndex,
                    const aiMesh* paiMesh,
                    std::vector<glm::vec3>& Positions,
                    std::vector<glm::vec3>& Normals,
                    std::vector<glm::vec2>& TexCoords,
                    std::vector<VertexBoneData>& Bones,
                    std::vector<unsigned int>& Indices);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();
    
    Assimp::Importer Importer;
    const aiScene* m_pScene;
    aiMatrix4x4 m_GlobalInverseTransform;

    GLuint gBones;
    GLuint gProjection;
    GLuint gView;
    GLuint gModel;

    GLuint gObjectLocation;
    GLuint gCameraParamsLocation;
    GLuint gPersProjParamsLocation;
    
    GLuint gWVP;

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
    unsigned int m_NumBones = 0;
    std::unordered_map<std::string, unsigned int> m_BoneMapping;
    std::vector<BoneInfo> m_BoneInfo;
    std::vector<VertexBoneData> Bones;
    std::vector<MaterialUniforms*> m_material;
};
