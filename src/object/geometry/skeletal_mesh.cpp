#include <object/geometry/skeletal_mesh.hpp>
#include <managers/global_state.hpp>


GeometrySkeletalMesh::GeometrySkeletalMesh(const std::string& Filename) 
    : GeometryMesh()
{
    GlobalState::MeshLoader->LoadMesh(Filename, *this);
    SetType(SKELETAL_MESH);
    glBindVertexArray(0);	
}

const Skeleton& GeometrySkeletalMesh::GetSkeleton() const {
    return skeleton;
}

GeometrySkeletalMesh* GeometrySkeletalMesh::Create(const std::string& name, const std::string& path) {
    auto [it, inserted] = cache.try_emplace(name, path);
    return &it->second;
}
GeometrySkeletalMesh* GeometrySkeletalMesh::Find(const std::string& name) {
    auto it = cache.find(name);
    return it != cache.end() ? &it->second : nullptr;
}

void GeometrySkeletalMesh::Delete(const std::string& path) {
    auto it = cache.find(path);
    if (it != cache.end()) {
        cache.erase(it); 
    }
}

void GeometrySkeletalMesh::ClearСache() {
    for (auto it = cache.begin(); it != cache.end(); ) {
        it = cache.erase(it);
    }
}
