#pragma once
#include "line.hpp"
#include "../game/gameManager.hpp"

template<std::size_t N>
class Sphere_wire : public Line
{
public:
    Sphere_wire()
        : Line()
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

        glUniform3f(gColorLocation, color.x, color.y, color.z);
        glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &static_cast<Sprite_rdata*>(RenderData)->matrix);

        GameManager::render.PushLineWidth(width);

        glDrawArrays(GL_LINE_STRIP, 0, geometryInfo.numVertices);
    }

    static void initializeGeometry() {
        std::vector<GLfloat> vertices;
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
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }


        geometryInfo.numVertices = static_cast<int>(vertices.size()) / 3;

        glGenVertexArrays(1, &geometryInfo.VAO);
        glBindVertexArray(geometryInfo.VAO);

        glGenBuffers(1, &geometryInfo.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, geometryInfo.VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

private:
    inline static struct GeometryInfo geometryInfo = {0, 0, 0, 0, 0};
    GLuint gColorLocation;
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
};
