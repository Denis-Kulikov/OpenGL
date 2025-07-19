#pragma once
#include "mesh.hpp"
#include "skeleton/bone/vertex_bone_data.hpp"
#include "skeleton/skeleton.hpp"

class GeometrySkeletalMesh : public GeometryMesh {
public:
    GeometrySkeletalMesh(const std::string& Filename);

    void InitBuffers();
    const Skeleton& GetSkeleton() const;
    
    static GeometrySkeletalMesh* Create(const std::string& name, const std::string& path);
    static GeometrySkeletalMesh* Find(const std::string &name);
    static void Delete(const std::string &name);
    static void ClearСache();


// protected:
    #define BONE_ID_LOCATION     3
    #define BONE_WEIGHT_LOCATION 4

    Skeleton skeleton;
    inline static std::unordered_map<std::string, GeometrySkeletalMesh> cache;
};

