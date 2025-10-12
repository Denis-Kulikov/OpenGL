#pragma once
#include "mesh_data.hpp"
#include "skeleton/bone/vertex_bone_data.hpp"
#include "skeleton/skeleton.hpp"

class SkeletalMeshData : public MeshData {
public:
    std::vector<VertexBoneData> Bones;
    std::shared_ptr<Skeleton> skeleton;
};
