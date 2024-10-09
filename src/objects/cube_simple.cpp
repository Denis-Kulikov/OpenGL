#include <object/cube_simple.hpp>
#include <game/gameManager.hpp>
#include <omp.h>

struct GeometryInfo CubeSimple::geometryInfo = {0, 0, 0, 0, 0};

CubeSimple::CubeSimple()
    : shader("shaders/base_fs.glsl", "shaders/instance_vs.glsl")
{
    gCommonMatrix = glGetUniformLocation(shader, "commonMatrix");;
    gColorLocation = glGetUniformLocation(shader, "gColor");
    assert(gCommonMatrix  != 0xFFFFFFFF);
    assert(gColorLocation != 0xFFFFFFFF);
}

void CubeSimple::Render(void *RenderData) const {
    CubeSimple_rdata *rdata = static_cast<CubeSimple_rdata*>(RenderData);

    glUseProgram(shader);
    glUniform3f(gColorLocation, color.x, color.y, color.z);
    glUniformMatrix4fv(gCommonMatrix, 1, GL_TRUE, &rdata->Matrix);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(2, 1);

    glDrawElementsInstanced(GL_TRIANGLES, geometryInfo.numIndices, GL_UNSIGNED_INT, 0, rdata->size);

    glDeleteBuffers(1, &rdata->instanceVBO);

    glBindVertexArray(0);
}

void CubeSimple::Drow(ull_I size, Matrix4f &Matrix, BigArray<bool> &data, int arrayIndex, int parts) {
    static std::vector<glm::vec3> cubes[NUM_TREADS];
    cubes[0].clear();
    float half = size / 2;
    ull_I arrayOffset = static_cast<ull_I>(size) * static_cast<ull_I>(size) * static_cast<ull_I>(size) / data.numParts;
    ull_I array_indexOffset = arrayOffset * arrayIndex;
    ull_I arrayZOffset = size / data.numParts * arrayIndex;

    if (arrayIndex % 2 == 0) std::cout << "Array " << arrayIndex << std::endl;
    for (ull_I partIndex = 0; partIndex < parts; ++partIndex) {
        if ((partIndex % 64 == 0) && (arrayIndex % 2 == 0)) std::cout << "  Part " << partIndex << std::endl;

        ull_I partOffset = arrayOffset / parts;
        ull_I zOffset = arrayOffset * arrayIndex;
        ull_I part_indexZOffset = partOffset * partIndex;
        ull_I sizePart = size / data.numParts / parts;
        ull_I partZOffset = size / data.numParts / parts * partIndex;

        #pragma omp parallel for num_threads(NUM_TREADS) schedule(static)
        for (ull_I z = 0; z < (size / data.numParts / parts); ++z) {
            int threadId = omp_get_thread_num();
            ull_I z_size = z * size * size; 
            for (ull_I y = 0; y < size; ++y) {
                ull_I y_size = y * size;
                for (ull_I x = 0; x < size; ++x) {
                    ull_I indexOffset = x + y_size + z_size + array_indexOffset +  part_indexZOffset;

                    if (data.get(indexOffset)) {
                        cubes[threadId].emplace_back(glm::vec3(
                            x - half,
                            y - half,
                            z - half + arrayZOffset + partZOffset
                        ));
                    }
                }
            }
        }
    
        glFinish();

        for (int i = 0; i < NUM_TREADS; ++i) {
            GameManager::render.PushGeometry(&geometryInfo);

            GLuint instanceVBO;
            glGenBuffers(1, &instanceVBO);
            glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
            glBufferData(GL_ARRAY_BUFFER, cubes[i].size() * sizeof(glm::vec3), cubes[i].data(), GL_STATIC_DRAW);
            CubeSimple_rdata rdata = {Matrix, instanceVBO, cubes[i].size()};

            Render(&rdata);
        }
    }
}


void CubeSimple::Drow(ull_I size, Matrix4f &Matrix, BitBigArray &data, int arrayIndex) {
    ull_I offset = static_cast<ull_I>(size) * static_cast<ull_I>(size) * static_cast<ull_I>(size) *
                   static_cast<ull_I>(arrayIndex) / data.numParts;
    static std::vector<glm::vec3> cubes[NUM_TREADS];

    if (arrayIndex % 32 == 0) std::cout << "Part " << arrayIndex << std::endl;

    float half = size / 2;

    cubes[0].clear();
    #pragma omp parallel for num_threads(NUM_TREADS) schedule(static)
    for (int z = 0; z < (size / data.numParts); ++z) {
        int threadId = omp_get_thread_num();
        uint8_t byte;
        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; x += 8) {
                ull_I indexOffset = static_cast<ull_I>(x) + 
                                    static_cast<ull_I>(y) * size + 
                                    static_cast<ull_I>(z) * size * size + 
                                    offset;

                if (!(byte = data.getByte(indexOffset))) 
                    continue;
                
                for (int i = 0; i < 8; ++i) {
                    if (BitArray::getBit(byte, i))
                        cubes[threadId].emplace_back(glm::vec3(x + i - half, y - half, z - half + (size / data.numParts * arrayIndex)));
                }
            }
        }
    }

    glFinish(); // Защита от переполнения памяти

    for (int i = 0; i < NUM_TREADS; ++i) {
        GameManager::render.PushGeometry(&geometryInfo);

        GLuint instanceVBO;
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, cubes[i].size() * sizeof(glm::vec3), cubes[i].data(), GL_STATIC_DRAW);
        CubeSimple_rdata rdata = {Matrix, instanceVBO, cubes[i].size()};

        Render(&rdata);
    }
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
