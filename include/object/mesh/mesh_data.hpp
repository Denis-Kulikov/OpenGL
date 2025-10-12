#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "../material/material.hpp"

class MeshData {
public:
    struct MeshEntry {
        MeshEntry() {
            NumIndices = 0;
            BaseVertex = 0;
            BaseIndex = 0;
        }

        void Draw() const {
            glDrawElementsBaseVertex(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 
                                    (void*)(sizeof(unsigned int) * BaseIndex), BaseVertex);
        }

        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        Material Material;
    };

    std::vector<unsigned int> Indices;
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec2> TexCoords;
    std::vector<glm::vec3> Normals;
    std::vector<MeshEntry> m_Entries;
};
