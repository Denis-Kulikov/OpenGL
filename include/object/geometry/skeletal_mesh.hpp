#pragma once
#include "mesh.hpp"

class GeometrySkeletalMesh : public GeometryMesh {
public:
    GeometrySkeletalMesh(const std::string& Filename);

    #define BONE_ID_LOCATION     3
    #define BONE_WEIGHT_LOCATION 4

    #define NUM_BONES_PER_VEREX 4

    struct VertexBoneData {
        void AddBoneData(unsigned int BoneID, float Weight);
        static const int size = NUM_BONES_PER_VEREX;
        unsigned int IDs[size];
        float Weights[size];
    };

    struct BoneInfo {
        BoneInfo() : BoneOffset(), FinalTransformation() {}
        glm::mat4x4 BoneOffset;
        glm::mat4x4 FinalTransformation;
    };

    virtual bool InitFromScene(const aiScene* m_pScene, const std::string& Filename);
    void LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones);

// Для анимаций
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform);

    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);


    unsigned int m_NumBones = 0;
    std::unordered_map<std::string, unsigned int> m_BoneMapping;
    std::vector<BoneInfo> m_BoneInfo;
};

