#pragma once
#include "../game/gameManager.hpp"

template<std::size_t N>
class Sphere : public Sprite
{
public:
    Sphere(const std::string &_name, const char *FS, const char *VS, const char *texturePath)
        : Sprite(_name, FS, VS, texturePath)
    {
        gColorLocation = glGetUniformLocation(shader, "gColor");
    }

    Sphere(const char *texturePath)
        : Sphere(std::string("None"), "shaders/sphere_fs.glsl", "shaders/sphere_vs.glsl", texturePath)
    {
        gColorLocation = glGetUniformLocation(shader, "gColor");
    }

    Sphere()
        : Sphere(std::string("None"), "shaders/sphere_fs.glsl", "shaders/sphere_vs.glsl", "")
    {
        gColorLocation = glGetUniformLocation(shader, "gColor");
    }

    struct GeometryInfo *GetGeometry() override {
        return &geometryInfo;
    }

    void SetColor(glm::vec3 _color) {
        color = _color;
    }

    void Render(void *RenderData) const {
        GameManager::render.PushGeometry(&geometryInfo);

        glUseProgram(shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(gTextureSamplerLocation, 0);

        glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &static_cast<Sprite_rdata*>(RenderData)->matrix);

        glDrawElements(GL_TRIANGLES, geometryInfo.numIndices, GL_UNSIGNED_INT, 0);
    }

    static void initializeGeometry() {
        struct Vertex {
            Vertex(glm::vec3 position, glm::vec2 texCoord)
                : position(position), texCoord(texCoord) {}
            glm::vec3 position;
            glm::vec2 texCoord;
        };
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        geometryInfo.numVertices = N;

        int numLat = N; // количество широт
        int numLon = N; // количество долгот

        for (int lat = 0; lat <= numLat; ++lat) {
            float theta = lat * M_PI / numLat; // угол широты
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int lon = 0; lon <= numLon; ++lon) {
                float phi = lon * 2 * M_PI / numLon; // угол долготы
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                // Координаты вершины
                float x = cosPhi * sinTheta;
                float y = cosTheta;
                float z = sinPhi * sinTheta;

                // Добавление вершин
                glm::vec3 position;
                position.x = x / 2; // делим на 2 для радиуса 0.5
                position.y = y / 2;
                position.z = z / 2;


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

private:
    inline static struct GeometryInfo geometryInfo = {0, 0, 0, 0, 0};
    GLuint gColorLocation;
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
};
