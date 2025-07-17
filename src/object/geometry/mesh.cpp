#include <object/geometry/mesh.hpp>
#include <managers/global_state.hpp>


GeometryMesh::GeometryMesh(const std::string& Filename)
{
    GlobalState::MeshLoader->LoadMesh(Filename, *this);
    SetType(MESH);
    glBindVertexArray(0);	
}

void GeometryMesh::Bind() const {
    glBindVertexArray(buffers[VAO]);
}

void GeometryMesh::Draw(void* data) const {
    int index = *static_cast<int*>(data);

    glDrawElementsBaseVertex(GL_TRIANGLES, 
                                m_Entries[index].NumIndices, 
                                GL_UNSIGNED_INT, 
                                (void*)(sizeof(unsigned int) * m_Entries[index].BaseIndex), 
                                m_Entries[index].BaseVertex);
}



GeometryMesh* GeometryMesh::Create(const std::string& name, const std::string& path) {
    auto [it, inserted] = cache.try_emplace(name, path);
    return &it->second;
}
GeometryMesh* GeometryMesh::Find(const std::string& name) {
    auto it = cache.find(name);
    return it != cache.end() ? &it->second : nullptr;
}

void GeometryMesh::Delete(const std::string& path) {
    auto it = cache.find(path);
    if (it != cache.end()) {
        cache.erase(it); 
    }
}

void GeometryMesh::ClearСache() {
    for (auto it = cache.begin(); it != cache.end(); ) {
        it = cache.erase(it);
    }
}
