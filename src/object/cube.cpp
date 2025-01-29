#include <object/cube.hpp>
#include <game/gameManager.hpp>

struct GeometryInfo Cube::geometryInfo = {0, 0, 0, 0, 0};

void Cube::Render(void *RenderData) const {
    GameManager::render.PushGeometry(&geometryInfo);

    glUseProgram(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(gTextureSamplerLocation, 0);

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &static_cast<Sprite_rdata*>(RenderData)->matrix);

    glDrawElements(GL_TRIANGLES, geometryInfo.numIndices, GL_UNSIGNED_INT, 0);
}

void Cube::initializeGeometry()
{
    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoord;
    };

    // Текстурные координаты назначены верх ногами
    std::vector<Vertex> vertices = {
        // передняя грань (начало: ближний сверху справа)
        {{0.5, 0.5, 0.5}, {0.5, 2.0f / 3.0f}},  {{-0.5, 0.5, 0.5}, {0.25f, 2.0f / 3.0f}},  {{-0.5, -0.5, 0.5}, {0.25f, 1.0f / 3.0f}},  {{0.5, -0.5, 0.5}, {0.5, 1.0f / 3.0f}},
        // задняя грань (начало: дальний сверху справа)
        {{0.5, 0.5, -0.5}, {0.75f, 2.0f / 3.0f}}, {{-0.5, 0.5, -0.5}, {1.0f, 2.0f / 3.0f}}, {{-0.5, -0.5, -0.5}, {1.0f, 1.0f / 3.0f}}, {{0.5, -0.5, -0.5}, {0.75f, 1.0f / 3.0f}},

        // левая грань (начало: дальний сверху слева)
        {{-0.5, 0.5, 0.5}, {0.25f, 2.0f / 3.0f}}, {{-0.5, 0.5, -0.5}, {0.0, 2.0f / 3.0f}}, {{-0.5, -0.5, -0.5}, {0.0, 1.0f / 3.0f}}, {{-0.5, -0.5, 0.5}, {0.25, 1.0f / 3.0f}},
        // правая грань (начало: ближний сверху справа)
        {{0.5, 0.5, 0.5}, {0.5f, 2.0f / 3.0f}},  {{0.5, 0.5, -0.5}, {0.75f, 2.0f / 3.0f}},  {{0.5, -0.5, -0.5}, {0.75f, 1.0f / 3.0f}},  {{0.5, -0.5, 0.5}, {0.5f, 1.0f / 3.0f}},

        // нижняя грань (начало: ближний снизу слева)
        {{-0.5, -0.5, 0.5}, {0.25f, 1.0f / 3.0f}}, {{0.5, -0.5, 0.5}, {0.5f, 1.0f / 3.0f}}, {{0.5, -0.5, -0.5}, {0.5f, 0.0f}},  {{-0.5, -0.5, -0.5}, {0.25f, 0.0f}},
        // верхняя грань (начало: ближний сверху слева)
        {{-0.5, 0.5, 0.5}, {0.25f, 2.0f / 3.0f}}, {{0.5, 0.5, 0.5}, {0.5f, 2.0f / 3.0f}},   {{0.5, 0.5, -0.5}, {0.5f, 1.0f}},   {{-0.5, 0.5, -0.5}, {0.25f, 1.0f}},
    };

    std::vector<GLuint> indices = {
        0,  1,  2,  0,  2,  3,   // передняя грань
        4,  5,  6,  4,  6,  7,   // задняя грань
        8,  9,  10, 8,  10, 11,  // левая грань
        12, 13, 14, 12, 14, 15,  // правая грань
        16, 17, 18, 16, 18, 19,  // нижняя грань
        20, 21, 22, 20, 22, 23   // верхняя грань
    };

    geometryInfo.numVertices = vertices.size();
    geometryInfo.numIndices = indices.size();

    glGenVertexArrays(1, &geometryInfo.VAO);
    glBindVertexArray(geometryInfo.VAO);

    glGenBuffers(1, &geometryInfo.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, geometryInfo.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &geometryInfo.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometryInfo.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    glBindVertexArray(0);
}

struct GeometryInfo *Cube::GetGeometry()
{
    return &geometryInfo;
}

Cube::Cube(const std::string &texture_path)
    : Sprite(std::string("None"), "shaders/cube_fs.glsl", "shaders/cube_vs.glsl", texture_path.c_str())
{}

Cube::Cube()
    : Sprite (std::string("None"), "shaders/cube_fs.glsl", "shaders/cube_vs.glsl", "img/skybox.png")
{}
