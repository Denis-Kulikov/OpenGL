#pragma once
#include "geometry.hpp"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <object/material/texture.hpp>
#include <iostream>

class GeometryMesh : public Geometry {
protected:
    GeometryMesh() {}

public:
    GeometryMesh(const std::string& Filename);

    virtual void Bind() const override;
    virtual void Draw(void* data = nullptr) const override;
    std::size_t size() {return m_Entries.size();}
    int GetTextureIndex(int index) {return m_Entries[index].MaterialIndex;}

    static GeometryMesh* Create(const std::string& name, const std::string& path);
    static GeometryMesh* Find(const std::string &name);
    static void Delete(const std::string &name);
    static void ClearСache();

    struct MeshEntry {
        MeshEntry() {
            NumIndices = 0;
            MaterialIndex = -1;
            BaseVertex = 0;
            BaseIndex = 0;
        }

        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };

protected:
    #define POSITION_LOCATION    0
    #define TEX_COORD_LOCATION   1
    #define NORMAL_LOCATION      2 // нужно изменить шейдеры

    enum VB_TYPES {
        VAO,
        EBO,
        POSITION_VB,
        TEXCOORD_VB,
        NORMAL_VB,
        NUM_VBs,
        BONE_VB = NUM_VBs,
        NUM_skeletal_VBs
    };


    void InitBuffers();
    bool LoadMesh(const std::string& Filename);
    virtual bool InitFromScene(const aiScene* m_pScene, const std::string& Filename);
    bool InitMaterials(const aiScene* scene, const std::string& directory);
    void InitMesh(unsigned int MeshIndex, const aiMesh* paiMesh,
                std::vector<glm::vec3>& Positions, std::vector<glm::vec3>& Normals,
                std::vector<glm::vec2>& TexCoords, std::vector<unsigned int>& Indices);

    inline static std::unordered_map<std::string, GeometryMesh> cache;
    std::vector<GLuint> buffers;
    std::vector<MeshEntry> m_Entries;

public:
    std::vector<Texture*> m_Textures;
};
