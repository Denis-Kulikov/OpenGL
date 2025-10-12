#pragma once
#include "mesh_loader.hpp"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class AssimpMeshLoader : public IMeshLoader {
public:
    bool LoadMesh(const std::string& fileName, MeshData& mesh) override;
    bool LoadMesh(const std::string& fileName, SkeletalMeshData& mesh) override;

protected:
    std::vector<int> InitVertexes(MeshData& mesh, const aiScene* m_pScene);
    bool InitFromScene(MeshData& mesh, const aiScene* m_pScene, const std::string& fileName);
    bool InitFromScene(SkeletalMeshData& mesh, const aiScene* m_pScene, const std::string& fileName);
    bool InitMaterials(MeshData& mesh, std::vector<int>& MaterialIndex, const aiScene* scene, const std::string& directory);
    void InitMesh(MeshData& mesh, unsigned int MeshIndex, const aiMesh* paiMesh);

    void LoadBones(SkeletalMeshData& mesh, unsigned int MeshIndex, const aiMesh* pMesh);
    bool LoadAnimations(SkeletalMeshData& mesh, const aiScene* m_pScene);
};
