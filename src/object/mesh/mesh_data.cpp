#include <object/mesh/mesh_data.hpp>
#include <object/material/material.hpp>

MeshData::MeshEntry::MeshEntry(unsigned int NumIndices, unsigned int BaseVertex, unsigned int BaseIndex, const Material* material)
    : NumIndices(NumIndices), BaseVertex(BaseVertex), BaseIndex(BaseIndex), material(std::make_shared<Material>(*material))
{}

MeshData::MeshEntry::MeshEntry() {
    NumIndices = 0;
    BaseVertex = 0;
    BaseIndex = 0;

    material = std::make_shared<Material>();
}

void MeshData::MeshEntry::Draw() const {
    glDrawElementsBaseVertex(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 
                            (void*)(sizeof(unsigned int) * BaseIndex), BaseVertex);
}

MeshData* MeshData::Create(const std::string& name) {
    auto [it, inserted] = cache.try_emplace(name);
    return &it->second;
}
MeshData* MeshData::Find(const std::string& name) {
    auto it = cache.find(name);
    return it != cache.end() ? &it->second : nullptr;
}
void MeshData::Delete(const std::string& path) {
    auto it = cache.find(path);
    if (it != cache.end()) {
        cache.erase(it); 
    }
}
void MeshData::ClearСache() {
    for (auto it = cache.begin(); it != cache.end(); ) {
        it = cache.erase(it);
    }
}