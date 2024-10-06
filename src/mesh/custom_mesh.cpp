#include <game/gameManager.hpp>
#include <mesh/custom_mesh.hpp>


CustomMesh::CustomMesh(vec3i size, BitBigArray &data, int partIndex)
    : Sprite("", "shaders/base_fs.glsl", "shaders/base_vs.glsl", "")
{
    gColorLocation = glGetUniformLocation(shader, "gColor");

    vertices.reserve(8 / 2 * 3 * (size.z / data.numParts));
    indices.reserve(6 / 2 * 6 * (size.z / data.numParts));
    
    initializeGeometry(size, data, partIndex);

    vertices.clear();
    indices.clear();
}

CustomMesh::~CustomMesh() {}

void CustomMesh::initializeGeometry(vec3i size, BitBigArray &data, int partIndex) {
    struct face {
        face()
            : f(6)
        {}
        BitArray f; // is face?
    };
    std::vector<face> faces(size.x * size.y * (size.z / data.numParts));
    
    if (size.x < 2 || size.y < 2 || size.z < 2) assert(false && "Wrong size");

    int offset = partIndex * size.x * size.y * (size.z / data.numParts);
    std::cout << "Part " << partIndex << std::endl;
   
    // for (int z = 0; z < (size.z / data.numParts); ++z) {          // FRONT BACK
    //     for (int y = 0; y < size.y; ++y) {      // BOTTOM TOP
    //         for (int x = 0; x < size.x; ++x) {  // LEFT RIGHT
    //             ull_I index = x + y * size.x + z * size.x * size.y;
    //             ull_I indexOffset = x + y * size.x + z * size.x * size.y + offset;
    //             // if (indexOffset < data.totalSize) std::cout << "T: " << x << " " << y << " " << z << std::endl;;
    //             if (data.getBit(indexOffset)) {
    //                 faces[index].f.setBit(Face::LEFT,     x == 0 || !data.getBit(indexOffset - 1));
    //                 faces[index].f.setBit(Face::RIGHT,    x == size.x - 1 || !data.getBit(indexOffset + 1));
    //                 faces[index].f.setBit(Face::TOP,      y == size.y - 1 || !data.getBit(indexOffset + size.x));
    //                 faces[index].f.setBit(Face::BOTTOM,   y == 0 || !data.getBit(indexOffset - size.x));
    //                 faces[index].f.setBit(Face::BACK,     z == size.z - 1 || !data.getBit(indexOffset + size.x * size.y));
    //                 faces[index].f.setBit(Face::FRONT,    z == 0 || !data.getBit(indexOffset - size.x * size.y));
    //             } else {
    //                 // if (partIndex) ++count;
    //                 for (int i = Face::LEFT; i <= Face::FRONT; ++i) 
    //                     faces[index].f.setBit(i, false);
    //             }
    //         }
    //     }
    // }


    for (int z = 0; z < (size.z / data.numParts); ++z) {
        for (int y = 0; y < size.y; ++y) {
            for (int x = 0; x < size.x; ++x) {
                // Проверяем, есть ли кубик на этой позиции
                ull_I index = x + y * size.x + z * size.x * size.y;
                ull_I indexOffset = x + y * size.x + z * size.x * size.y + offset;
                if (data.getBit(indexOffset)) {
                    // // Задний план
                    // glm::vec3 v0 = glm::vec3(x, y, z + (size.z / data.numParts * partIndex));          // Левая нижняя
                    // glm::vec3 v1 = glm::vec3(x + 1, y, z + (size.z / data.numParts * partIndex));      // правая нижняя
                    // glm::vec3 v2 = glm::vec3(x + 1, y + 1, z + (size.z / data.numParts * partIndex));  // правая верхняя
                    // glm::vec3 v3 = glm::vec3(x, y + 1, z + (size.z / data.numParts * partIndex));      // Левая верхняя
                    // // Передний план
                    // glm::vec3 v4 = glm::vec3(x, y, z + 1 + (size.z / data.numParts * partIndex));           // Левая нижняя
                    // glm::vec3 v5 = glm::vec3(x + 1, y, z + 1 + (size.z / data.numParts * partIndex));       // правая нижняя
                    // glm::vec3 v6 = glm::vec3(x + 1, y + 1, z + 1 + (size.z / data.numParts * partIndex));   // правая верхняя
                    // glm::vec3 v7 = glm::vec3(x, y + 1, z + 1 + (size.z / data.numParts * partIndex));       // Левая верхняя

                    // // if (faces[index].f.getBit(Face::LEFT)) 
                    //     addFace(vertices, indices, v0, v3, v7, v4); // Левая грань
                    // // if (faces[index].f.getBit(Face::RIGHT)) 
                    //     addFace(vertices, indices, v1, v5, v6, v2); // Правая грань
                    // // if (faces[index].f.getBit(Face::BOTTOM)) 
                    //     addFace(vertices, indices, v0, v1, v5, v4); // Нижняя грань
                    // // if (faces[index].f.getBit(Face::TOP)) 
                    //     addFace(vertices, indices, v3, v2, v6, v7); // Верхняя грань
                    // // if (faces[index].f.getBit(Face::FRONT)) 
                    //     addFace(vertices, indices, v0, v1, v2, v3); // Передняя грань
                    // // if (faces[index].f.getBit(Face::BACK)) 
                    //     addFace(vertices, indices, v4, v5, v6, v7); // Задняя грань

                    addCube(vertices, indices, glm::vec3(x, y, z + (size.z / data.numParts * partIndex)));
                }
            }
        }
    }

    geometryInfo.numVertices = vertices.size();
    geometryInfo.numIndices  = indices.size();

    glGenVertexArrays(1, &geometryInfo.VAO);
    glBindVertexArray(geometryInfo.VAO);

    glGenBuffers(1, &geometryInfo.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, geometryInfo.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &geometryInfo.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometryInfo.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
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

struct GeometryInfo *CustomMesh::GetGeometry() {
    return &geometryInfo;
}

void CustomMesh::Render(void *RenderData) const {
    GameManager::render.PushGeometry(&geometryInfo);
    glUseProgram(shader);
    glUniform3f(gColorLocation, color.x, color.y, color.z);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &static_cast<Sprite::Sprite_rdata*>(RenderData)->matrix);
    glDrawElements(GL_TRIANGLES, geometryInfo.numIndices, GL_UNSIGNED_INT, 0);
}
