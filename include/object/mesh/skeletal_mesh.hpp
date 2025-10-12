#pragma once
#include "skeletal_mesh_data.hpp"
#include "mesh.hpp"

class SkeletalMesh : public Mesh {
public:
    SkeletalMesh(const SkeletalMeshData& meshData, Shader* shader);

    const Skeleton& GetSkeleton() const;
    
    static SkeletalMesh* Create(const std::string& name, const SkeletalMeshData& meshData, Shader* shader);
    static SkeletalMesh* Find(const std::string &name);
    static void Delete(const std::string &name);
    static void ClearСache();

    std::shared_ptr<Skeleton> skeleton;
    inline static std::unordered_map<std::string, SkeletalMesh> cache;
};
