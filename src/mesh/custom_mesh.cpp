#include <omp.h>
#include <game/gameManager.hpp>
#include <mesh/custom_mesh.hpp>


CustomMesh::CustomMesh(vec3i size, BitBigArray &data, int partIndex)
    : Sprite("", "shaders/base_fs.glsl", "shaders/base_vs.glsl", "")
{
    gColorLocation = glGetUniformLocation(shader, "gColor");

    // for (int i = 0; i < NUM_TREADS; ++i) {
    //     vertices[i].reserve((8 * 3 * (size.z / data.numParts)) / NUM_TREADS);
    //     indices[i].reserve((6 * 6 * (size.z / data.numParts)) / NUM_TREADS);
    // }
    
    initializeGeometry(size, data, partIndex);

    for (int i = 0; i < NUM_TREADS; ++i) {
        vertices[i].clear();
        indices[i].clear();
    }
}

CustomMesh::~CustomMesh() {}

void CustomMesh::initializeGeometry(vec3i size, BitBigArray &data, int partIndex) {
    ull_I offset = partIndex * size.x * size.y * size.z / data.numParts;

    if (partIndex % 32 == 0)
        std::cout << "Part " << partIndex << std::endl;
    // GameManager::Time.Update();
    // std::cout << "Create " << GameManager::Time.GetCurrentTime() << std::endl;

    glm::vec3 half(size.x / 2, size.y / 2, size.z / 2);

    #pragma omp parallel for num_threads(NUM_TREADS) schedule(static)
    for (int z = 0; z < (size.z / data.numParts); ++z) {
        int threadId = omp_get_thread_num();
        uint8_t byte;
        for (int y = 0; y < size.y; ++y) {
            for (int x = 0; x < size.x; x += 8) {
                ull_I indexOffset = static_cast<ull_I>(x) + 
                                    static_cast<ull_I>(y) * size.x + 
                                    static_cast<ull_I>(z) * size.x * size.y + 
                                    offset;
                                    
                if (!(byte = data.getByte(indexOffset))) 
                    continue;
                
                for (int i = 0; i < 8; ++i) {
                    if (BitArray::getBit(byte, i))
                        addCube(vertices[threadId], indices[threadId], glm::vec3(x + i- half.x, y - half.y, z - half.z + (size.z / data.numParts * partIndex)));
                }
            }
        }
    }

    glFinish(); // Защита от переполнения памяти


    // GameManager::Time.Update();
    // std::cout << "Buffers " << GameManager::Time.GetCurrentTime() << std::endl;

    for (int i = 0; i < NUM_TREADS; ++i) {
        geometryInfo[i].numVertices = vertices[i].size();
        geometryInfo[i].numIndices  = indices[i].size();

        glGenVertexArrays(1, &geometryInfo[i].VAO);
        glBindVertexArray(geometryInfo[i].VAO);

        glGenBuffers(1, &geometryInfo[i].VBO);
        glBindBuffer(GL_ARRAY_BUFFER, geometryInfo[i].VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices[i].size() * sizeof(float), vertices[i].data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &geometryInfo[i].EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometryInfo[i].EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[i].size() * sizeof(unsigned int), indices[i].data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    // GameManager::Time.Update();
    // std::cout << "Exit " << GameManager::Time.GetCurrentTime() << std::endl;
}

void CustomMesh::addCube(std::vector<float>& vertices, std::vector<unsigned int>& indices, glm::vec3 pos) {
    // Добавляем 8 вершин для куба (каждая грань будет использовать часть этих вершин)
    vertices.insert(vertices.end(), {
        pos.x - 0.5f, pos.y - 0.5f, pos.z + 0.5f,  // 0: Левая нижняя передняя
        pos.x + 0.5f, pos.y - 0.5f, pos.z + 0.5f,  // 1: Правая нижняя передняя
        pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f,  // 2: Правая верхняя передняя
        pos.x - 0.5f, pos.y + 0.5f, pos.z + 0.5f,  // 3: Левая верхняя передняя
        pos.x - 0.5f, pos.y - 0.5f, pos.z - 0.5f,  // 4: Левая нижняя задняя
        pos.x + 0.5f, pos.y - 0.5f, pos.z - 0.5f,  // 5: Правая нижняя задняя
        pos.x + 0.5f, pos.y + 0.5f, pos.z - 0.5f,  // 6: Правая верхняя задняя
        pos.x - 0.5f, pos.y + 0.5f, pos.z - 0.5f   // 7: Левая верхняя задняя
    });

    // Индексы для граней куба (каждая грань состоит из 2 треугольников)
    unsigned int baseIndex = vertices.size() / 3 - 8; // последние 8 добавленных вершин
    indices.insert(indices.end(), {
        // Передняя грань
        baseIndex + 0, baseIndex + 1, baseIndex + 2,  baseIndex + 0, baseIndex + 2, baseIndex + 3,
        // Задняя грань
        baseIndex + 4, baseIndex + 5, baseIndex + 6,  baseIndex + 4, baseIndex + 6, baseIndex + 7,
        // Левая грань
        baseIndex + 0, baseIndex + 4, baseIndex + 7,  baseIndex + 0, baseIndex + 7, baseIndex + 3,
        // Правая грань
        baseIndex + 1, baseIndex + 5, baseIndex + 6,  baseIndex + 1, baseIndex + 6, baseIndex + 2,
        // Нижняя грань
        baseIndex + 0, baseIndex + 1, baseIndex + 5,  baseIndex + 0, baseIndex + 5, baseIndex + 4,
        // Верхняя грань
        baseIndex + 3, baseIndex + 2, baseIndex + 6,  baseIndex + 3, baseIndex + 6, baseIndex + 7
    });
}

void CustomMesh::addFace(std::vector<float>& vertices, std::vector<unsigned int>& indices,
                         glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
    // Добавляем 4 вершины для одной грани
    vertices.insert(vertices.end(), {
        v0.x, v0.y, v0.z,
        v1.x, v1.y, v1.z,
        v2.x, v2.y, v2.z,
        v3.x, v3.y, v3.z
    });

    // Индексы для двух треугольников, образующих квадратную грань
    unsigned int baseIndex = vertices.size() / 3 - 4; // последние 4 добавленные вершины
    indices.insert(indices.end(), {
        baseIndex, baseIndex + 1, baseIndex + 2,  // первый треугольник (v0, v1, v2)
        baseIndex, baseIndex + 2, baseIndex + 3   // второй треугольник (v0, v2, v3)
    });
}

void CustomMesh::Render(void *RenderData) const {
    for (int i = 0; i < NUM_TREADS; ++i) {
        GameManager::render.PushGeometry(&geometryInfo[i]);
        glUseProgram(shader);
        glUniform3f(gColorLocation, color.x, color.y, color.z);
        glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &static_cast<Sprite::Sprite_rdata*>(RenderData)->matrix);
        glDrawElements(GL_TRIANGLES, geometryInfo[i].numIndices, GL_UNSIGNED_INT, 0);
    }
}
