#include <object/cube_simple.hpp>
#include <game/gameManager.hpp>

struct GeometryInfo CubeSimple::geometryInfo = {0, 0, 0, 0, 0};

CubeSimple::CubeSimple()
    : shader("shaders/base_fs.glsl", "shaders/instance_vs.glsl")
{
    gCommonMatrix = glGetUniformLocation(shader, "commonMatrix");;
    gColorLocation = glGetUniformLocation(shader, "gColor");
}

void CubeSimple::Render(void *RenderData) const {
    // GameManager::render.PushGeometry(&geometryInfo);
    // glUseProgram(shader);
    // glUniform3f(gColorLocation, color.x, color.y, color.z);
    // glUniformMatrix4fv(gCommonMatrix, 1, GL_TRUE, &static_cast<CubeSimple_rdata*>(RenderData)->Matrix);
    // glDrawElements(GL_TRIANGLES, geometryInfo.numIndices, GL_UNSIGNED_INT, 0);

    glUseProgram(shader);
    glUniform3f(gColorLocation, color.x, color.y, color.z);
    glUniformMatrix4fv(gCommonMatrix, 1, GL_TRUE, &static_cast<CubeSimple_rdata*>(RenderData)->Matrix);

    std::size_t size = static_cast<CubeSimple_rdata*>(RenderData)->size;
    BitArray *data = static_cast<CubeSimple_rdata*>(RenderData)->cubes;
    std::vector<glm::vec3> cubes;
    for (int z = 0; z < size; ++z) {
        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                if (data->get(x + y * size + z * size * size)) {
                    cubes.emplace_back(glm::vec3(x, y, z));
                }
            }
        }
    }

    GLuint instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, cubes.size() * sizeof(glm::vec3), cubes.data(), GL_STATIC_DRAW);

    glBindVertexArray(geometryInfo.VAO);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(2, 1);

    glDrawElementsInstanced(GL_TRIANGLES, geometryInfo.numIndices, GL_UNSIGNED_INT, 0, cubes.size());

    glDeleteBuffers(1, &instanceVBO);

    glBindVertexArray(0);
}

void CubeSimple::initializeGeometry()
{
    struct Vertex {
        glm::vec3 position;
    };

    std::vector<Vertex> vertices = {
        {{0.5, 0.5, 0.5}},  {{-0.5, 0.5, 0.5}},  {{-0.5, -0.5, 0.5}},  {{0.5, -0.5, 0.5}},
        {{0.5, 0.5, -0.5}}, {{-0.5, 0.5, -0.5}}, {{-0.5, -0.5, -0.5}}, {{0.5, -0.5, -0.5}},

        {{-0.5, 0.5, 0.5}}, {{-0.5, 0.5, -0.5}}, {{-0.5, -0.5, -0.5}}, {{-0.5, -0.5, 0.5}},
        {{0.5, 0.5, 0.5}},  {{0.5, 0.5, -0.5}},  {{0.5, -0.5, -0.5}},  {{0.5, -0.5, 0.5}},

        {{-0.5, -0.5, 0.5}}, {{0.5, -0.5, 0.5}}, {{0.5, -0.5, -0.5}},  {{-0.5, -0.5, -0.5}},
        {{-0.5, 0.5, 0.5}},  {{0.5, 0.5, 0.5}},   {{0.5, 0.5, -0.5}},   {{-0.5, 0.5, -0.5}}
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

    glBindVertexArray(0);
}

struct GeometryInfo *CubeSimple::GetGeometry()
{
    return &geometryInfo;
}
