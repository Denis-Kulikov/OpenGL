#include <game/gameManager.hpp>
#include <mesh/custom_mesh.hpp>

CustomMesh::CustomMesh(vec3i size, std::vector<bool> &data)
    : Sprite("", "shaders/base_fs.glsl", "shaders/base_vs.glsl", "")
{
    gColorLocation = glGetUniformLocation(shader, "gColor");
    initializeGeometry(size, data);
}


void CustomMesh::initializeGeometry(vec3i size, std::vector<bool> &data) {
    struct face {
        int f[6]; // is face?
    };
    std::vector<face> faces(size.x * size.y * size.z);
    
    if (size.x < 2 || size.y < 2 || size.z < 2) assert(false && "Wrong size");

    for (int z = 0; z < size.z; ++z) {          // FRONT BACK
        for (int y = 0; y < size.y; ++y) {      // BOTTOM TOP
            for (int x = 0; x < size.x; ++x) {  // LEFT RIGHT
                #define CELL(TARGET, X, Y, Z) TARGET[X + Y * size.x + Z * size.x * size.y]
                #define CUR(TARGET) CELL(TARGET, x, y, z)
                if (CUR(data)) {
                    CUR(faces).f[Face::LEFT]    = x == 0 || !CELL(data, x - 1, y, z);
                    CUR(faces).f[Face::RIGHT]   = x == size.x - 1 || !CELL(data, (x + 1), y, z);
                    CUR(faces).f[Face::TOP]     = y == size.y - 1 || !CELL(data, x, (y + 1), z);
                    CUR(faces).f[Face::BOTTOM]  = y == 0 || !CELL(data, x, (y - 1), z);
                    CUR(faces).f[Face::FRONT]   = z == size.z - 1 || !CELL(data, x, y, (z + 1));
                    CUR(faces).f[Face::BACK]    = z == 0 || !CELL(data, x, y, (z - 1));
                } else {
                    std::fill(std::begin(CUR(faces).f), std::end(CUR(faces).f), 0);
                }

                std::cout << "At " << x << ", " << y << ", " << z << " | " << 
                    CUR(faces).f[Face::LEFT]   << " " <<
                    CUR(faces).f[Face::RIGHT]  << " " <<
                    CUR(faces).f[Face::TOP]    << " " <<
                    CUR(faces).f[Face::BOTTOM] << " " <<
                    CUR(faces).f[Face::FRONT]  << " " <<
                    CUR(faces).f[Face::BACK]   << std::endl;
                if (x == 0 && y == 1 && z == 1) {
                    std::cout << CUR(faces).f[Face::TOP] << " " << CUR(faces).f[Face::BACK] <<  " "
                    << CELL(data, x, (y + 1), z) << " " << CELL(data, x, y, (z - 1)) << std::endl;
                }

            }
        }
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
        std::vector<float> normals;

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            for (int z = 0; z < size.z; ++z) {
                // Проверяем, есть ли кубик на этой позиции
                if (CELL(data, x, y, z)) {
                    // Задний план
                    glm::vec3 v0 = glm::vec3(x, y, z);          // Левая нижняя
                    glm::vec3 v1 = glm::vec3(x + 1, y, z);      // правая нижняя
                    glm::vec3 v2 = glm::vec3(x + 1, y + 1, z);  // правая верхняя
                    glm::vec3 v3 = glm::vec3(x, y + 1, z);      // Левая верхняя
                    // Передний план
                    glm::vec3 v4 = glm::vec3(x, y, z + 1);           // Левая нижняя
                    glm::vec3 v5 = glm::vec3(x + 1, y, z + 1);       // правая нижняя
                    glm::vec3 v6 = glm::vec3(x + 1, y + 1, z + 1);   // правая верхняя
                    glm::vec3 v7 = glm::vec3(x, y + 1, z + 1);       // Левая верхняя

                    if (CUR(faces).f[Face::LEFT]) 
                        addFace(vertices, indices, v0, v3, v7, v4); // Левая грань
                    
                    if (CUR(faces).f[Face::RIGHT]) 
                        addFace(vertices, indices, v1, v5, v6, v2); // Правая грань
                    
                    if (CUR(faces).f[Face::BOTTOM]) 
                        addFace(vertices, indices, v0, v1, v5, v4); // Нижняя грань
                    
                    if (CUR(faces).f[Face::TOP]) 
                        addFace(vertices, indices, v3, v2, v6, v7); // Верхняя грань
                    
                    if (CUR(faces).f[Face::FRONT]) 
                        addFace(vertices, indices, v0, v1, v2, v3); // Передняя грань
                    
                    if (CUR(faces).f[Face::BACK]) 
                        addFace(vertices, indices, v4, v5, v6, v7); // Задняя грань
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
    glActiveTexture(GL_TEXTURE0);
    glUniform3f(gColorLocation, color.x, color.y, color.z);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &static_cast<Sprite::Sprite_rdata*>(RenderData)->matrix);
    glDrawElements(GL_TRIANGLES, geometryInfo.numIndices, GL_UNSIGNED_INT, 0);
}
