#include <object/mesh/skeletal_mesh.hpp>
#include <managers/global.hpp>

SkeletalMesh::SkeletalMesh(const SkeletalMeshData& meshData, Shader* shader)
    : Mesh(static_cast<const MeshData&>(meshData), shader), skeleton(meshData.skeleton)
{
    glBindVertexArray(vao);
    InitAttributes(meshData);
    glBindVertexArray(0);	
}
SkeletalMesh::SkeletalMesh(const std::string& path, Shader* shader)
    : Mesh(path, shader)
{
    glBindVertexArray(vao);
    SkeletalMeshData meshData;
    GlobalState::MeshLoader->LoadMesh(path, meshData);
    InitAttributes(meshData);
    skeleton = meshData.skeleton;
    glBindVertexArray(0);	
}

const Skeleton& SkeletalMesh::GetSkeleton() const {
    return *skeleton.get();
}

void SkeletalMesh::InitAttributes(const SkeletalMeshData& meshData) {
    AddAttribute(meshData.Bones.data(), meshData.Bones.size() * sizeof(VertexBoneData),
                "aBoneIDs", 4, GL_INT, GL_FALSE, sizeof(VertexBoneData), offsetof(VertexBoneData, IDs));
    AddAttribute(meshData.Bones.data(), meshData.Bones.size() * sizeof(VertexBoneData),
                "aWeights", 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), offsetof(VertexBoneData, Weights));
}


SkeletalMesh* SkeletalMesh::Create(const std::string& name, const SkeletalMeshData& meshData, Shader* shader) {
    auto [it, inserted] = cache.try_emplace(name, meshData, shader);
    return &it->second;
}
SkeletalMesh* SkeletalMesh::Create(const std::string& name, const std::string& path, Shader* shader) {
    auto [it, inserted] = cache.try_emplace(name, path, shader);
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
