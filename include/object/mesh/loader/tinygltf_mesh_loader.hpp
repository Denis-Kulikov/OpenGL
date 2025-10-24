#pragma once
#include "mesh_loader.hpp"

namespace tinygltf {
   class Model;
};

// НЕ РАБОТАЕТ ДЛЯ АНИМИРОВАННЫХ МОДЕЛЕЙ
class TinygltfMeshLoader : public IMeshLoader {
public:
   bool LoadMesh(const std::string& fileName, MeshData& mesh) override;
   bool LoadMesh(const std::string& fileName, SkeletalMeshData& mesh) override;

protected:
   bool GetModel(const std::string& fileName, tinygltf::Model& model);

   bool TinygltfMeshLoader::InitFromScene(MeshData& mesh, const tinygltf::Model& model, const std::string& fileName);
};
