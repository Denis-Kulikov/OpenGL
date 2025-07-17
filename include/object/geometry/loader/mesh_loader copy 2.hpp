#pragma once
#include "../skeletal_mesh.hpp"

class IMeshLoader {
public:
    virtual void LoadMesh(const std::string& path2mesh, GeometryMesh& mesh) = 0;
    virtual void LoadMesh(const std::string& path2mesh, GeometrySkeletalMesh& mesh) = 0;
};
