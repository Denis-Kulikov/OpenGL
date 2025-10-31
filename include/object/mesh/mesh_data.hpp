#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>
#include <string>
#include <unordered_map>

class Material;

class MeshData {
public:
    struct MeshEntry {
        MeshEntry(unsigned int NumIndices, unsigned int BaseVertex, unsigned int BaseIndex, const Material* material);
        MeshEntry();
        void Draw() const;

        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        std::shared_ptr<Material> material;
    };

    std::vector<unsigned int> Indices;
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec2> TexCoords;
    std::vector<glm::vec3> Normals;
    std::vector<MeshEntry> m_Entries;

    static MeshData* Create(const std::string &name);
    static MeshData* Find(const std::string &name);
    static void Delete(const std::string &name);
    static void ClearСache();
    inline static std::unordered_map<std::string, MeshData> cache;
};
