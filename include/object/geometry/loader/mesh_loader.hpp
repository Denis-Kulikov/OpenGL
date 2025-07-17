#pragma once
#include "../skeletal_mesh.hpp"

class IMeshLoader {
public:
    virtual bool LoadMesh(const std::string& fileName, GeometryMesh& mesh) = 0;
    virtual bool LoadMesh(const std::string& fileName, GeometrySkeletalMesh& mesh) = 0;
};
