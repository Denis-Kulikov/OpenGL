#pragma once
#include "mesh_loader.hpp"

class AssimpMeshLoader : public IMeshLoader {
public:
    bool LoadMesh(const std::string& fileName, GeometryMesh& mesh) override;
    bool LoadMesh(const std::string& fileName, GeometrySkeletalMesh& mesh) override;

    void InitBuffers(GeometryMesh& mesh);
    void InitBuffers(GeometrySkeletalMesh& mesh);

    bool InitFromScene(GeometryMesh& mesh, const aiScene* m_pScene, const std::string& Filename);
    bool InitFromScene(GeometrySkeletalMesh& mesh, const aiScene* m_pScene, const std::string& Filename);

    bool InitMaterials(GeometryMesh& mesh, const aiScene* scene, const std::string& directory);
    void InitMesh(GeometryMesh& mesh, unsigned int MeshIndex, const aiMesh* paiMesh,
                std::vector<glm::vec3>& Positions, std::vector<glm::vec3>& Normals,
                std::vector<glm::vec2>& TexCoords, std::vector<unsigned int>& Indices);

    void LoadBones(GeometrySkeletalMesh& mesh, unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones);
    bool LoadAnimations(GeometrySkeletalMesh& mesh, const aiScene* m_pScene);
};
