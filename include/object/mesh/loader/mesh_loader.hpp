#pragma once
#include "../skeletal_mesh_data.hpp"

class IMeshLoader {
public:
    virtual bool LoadMesh(const std::string& fileName, MeshData& mesh) = 0;
    virtual bool LoadMesh(const std::string& fileName, SkeletalMeshData& mesh) = 0;
};
