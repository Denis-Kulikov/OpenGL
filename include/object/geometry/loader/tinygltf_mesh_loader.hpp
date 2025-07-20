#pragma once
#include "mesh_loader.hpp"

namespace tinygltf {
    class Model;
};

class TinygltfMeshLoader : public IMeshLoader {
public:
    bool LoadMesh(const std::string& fileName, GeometryMesh& mesh) override;
    bool LoadMesh(const std::string& fileName, GeometrySkeletalMesh& mesh) override;

protected:
    bool GetModel(const std::string& fileName, tinygltf::Model& model);

    bool TinygltfMeshLoader::InitFromScene(GeometryMesh& mesh, const tinygltf::Model& model, const std::string& fileName);
};
