#include <managers/render_manager.hpp>
#include <object/geometry/primitive/cube.hpp>


Cube::Cube() {
    SetType(PRIMITIVE);

    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoord;
    };

    const float size = .5f;
    std::vector<Vertex> vertices = {
        // передняя грань (начало: ближний сверху справа)
        {{ size,  size,  size}, {0.5f, 2.0f / 3.0f}},  {{-size,  size,  size}, {0.25f, 2.0f / 3.0f}},  {{-size, -size,  size}, {0.25f, 1.0f / 3.0f}},  {{ size, -size,  size}, {0.5f, 1.0f / 3.0f}},
        // задняя грань (начало: дальний сверху справа)
        {{ size,  size, -size}, {0.75f, 2.0f / 3.0f}}, {{-size,  size, -size}, {1.0f, 2.0f / 3.0f}}, {{-size, -size, -size}, {1.0f, 1.0f / 3.0f}}, {{ size, -size, -size}, {0.75f, 1.0f / 3.0f}},

        // левая грань (начало: дальний сверху слева)
        {{-size,  size,  size}, {0.25f, 2.0f / 3.0f}}, {{-size,  size, -size}, {0.0f, 2.0f / 3.0f}}, {{-size, -size, -size}, {0.0f, 1.0f / 3.0f}}, {{-size, -size,  size}, {0.25f, 1.0f / 3.0f}},
        // правая грань (начало: ближний сверху справа)
        {{ size,  size,  size}, {0.5f, 2.0f / 3.0f}},  {{ size,  size, -size}, {0.75f, 2.0f / 3.0f}},  {{ size, -size, -size}, {0.75f, 1.0f / 3.0f}},  {{ size, -size,  size}, {0.5f, 1.0f / 3.0f}},

        // нижняя грань (начало: ближний снизу слева)
        {{-size, -size,  size}, {0.25f, 1.0f / 3.0f}}, {{ size, -size,  size}, {0.5f, 1.0f / 3.0f}}, {{ size, -size, -size}, {0.5f, 0.0f}},  {{-size, -size, -size}, {0.25f, 0.0f}},
        // верхняя грань (начало: ближний сверху слева)
        {{-size,  size,  size}, {0.25f, 2.0f / 3.0f}}, {{ size,  size,  size}, {0.5f, 2.0f / 3.0f}},   {{ size,  size, -size}, {0.5f, 1.0f}},   {{-size,  size, -size}, {0.25f, 1.0f}},
    };


    std::vector<GLuint> indices = {
        0,  1,  2,  0,  2,  3,   // передняя грань
        4,  5,  6,  4,  6,  7,   // задняя грань
        8,  9,  10, 8,  10, 11,  // левая грань
        12, 13, 14, 12, 14, 15,  // правая грань
        16, 17, 18, 16, 18, 19,  // нижняя грань
        20, 21, 22, 20, 22, 23   // верхняя грань
    };

    numVertices = vertices.size();
    numIndices = indices.size();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    glBindVertexArray(0);
}
