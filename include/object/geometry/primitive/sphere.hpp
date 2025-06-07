#pragma once
#include "../primitive.hpp"


template<std::size_t N>
class Sphere : public GeometryPrimitive
{
public:

    Sphere() {
        SetType(PRIMITIVE);

        struct Vertex {
            Vertex(glm::vec3 position, glm::vec2 texCoord)
                : position(position), texCoord(texCoord) {}
            glm::vec3 position;
            glm::vec2 texCoord;
        };
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        numVertices = N;

        int numLat = N; // количество широт
        int numLon = N; // количество долгот

        for (int lat = 0; lat <= numLat; ++lat) {
            float theta = lat * glm::pi<float>() / numLat; // угол широты
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int lon = 0; lon <= numLon; ++lon) {
                float phi = lon * 2 * glm::pi<float>() / numLon; // угол долготы
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                // Координаты вершины
                float x = cosPhi * sinTheta;
                float y = cosTheta;
                float z = sinPhi * sinTheta;

                // Добавление вершин
                glm::vec3 position;
                position.x = x;
                position.y = y;
                position.z = z;


                // Текстурные координаты
                glm::vec2 texCoord;
                float u = static_cast<float>(lon) / numLon;
                float v = static_cast<float>(lat) / numLat;
                texCoord.x = u;
                texCoord.y = v;

                vertices.emplace_back(position, texCoord);
            }
        }

        // Генерация индексов
        for (int lat = 0; lat < numLat; ++lat) {
            for (int lon = 0; lon < numLon; ++lon) {
                int first = lat * (numLon + 1) + lon;
                int second = first + numLon + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }

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
};