#include <object/mesh/skeletal_mesh.hpp>
#include <array>

SkeletalMesh::SkeletalMesh(const SkeletalMeshData& meshData, Shader* shader)
    : Mesh(static_cast<const MeshData&>(meshData), shader), skeleton(meshData.skeleton)
{
    glBindVertexArray(vao);

    AddAttribute(meshData.Bones.data(), meshData.Bones.size() * sizeof(VertexBoneData),
                "aBoneIDs", 4, GL_INT, GL_FALSE, sizeof(VertexBoneData), offsetof(VertexBoneData, IDs));
    AddAttribute(meshData.Bones.data(), meshData.Bones.size() * sizeof(VertexBoneData),
                "aWeights", 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), offsetof(VertexBoneData, Weights));

    // std::vector<std::array<int, 4>> ids;
    // std::vector<std::array<float, 4>> weights;

    // ids.reserve(meshData.Bones.size());
    // weights.reserve(meshData.Bones.size());

    // for (auto it : meshData.Bones) {
    //     ids.push_back(it.IDs);
    //     weights.push_back(it.Weights);
    // }

    // AddAttribute(ids.data(), ids.size() * sizeof(glm::vec3), "aBoneIDs", 4, GL_INT); // нет в шейдере
    // AddAttribute(weights.data(), weights.size() * sizeof(glm::vec3), "aWeights", 4); // нет в шейдере


    glBindVertexArray(0);	
}

const Skeleton& SkeletalMesh::GetSkeleton() const {
    return *skeleton.get();
}

SkeletalMesh* SkeletalMesh::Create(const std::string& name, const SkeletalMeshData& meshData, Shader* shader) {
    auto [it, inserted] = cache.try_emplace(name, meshData, shader);
    return &it->second;
}
SkeletalMesh* SkeletalMesh::Find(const std::string& name) {
    auto it = cache.find(name);
    return it != cache.end() ? &it->second : nullptr;
}
void SkeletalMesh::Delete(const std::string& path) {
    auto it = cache.find(path);
    if (it != cache.end()) {
        cache.erase(it); 
    }
}
void SkeletalMesh::ClearСache() {
    for (auto it = cache.begin(); it != cache.end(); ) {
        it = cache.erase(it);
    }
}
