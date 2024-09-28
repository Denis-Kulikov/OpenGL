#pragma once
#include "line.hpp"

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
        if (GameManager::render->pipeline.camera == nullptr) {
            std::cout << "Error Render: not found camera" << std::endl;
            return;
        }

        GameManager::render->PushGeometry(&geometryInfo);

        glUseProgram(shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(gTextureSamplerLocation, 0);

        glUniform3f(gColorLocation, color.x, color.y, color.z);
        glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &static_cast<Sprite_rdata*>(RenderData)->matrix);

        if (GameManager::render->LineWidth != width) {
            GameManager::render->LineWidth = width;
            glLineWidth(GameManager::render->LineWidth);
        }
        
        glDrawArrays(GL_LINE_STRIP, 0, geometryInfo.numVertices);
    }

    static void initializeGeometry() {
        std::vector<GLfloat> vertices;
        geometryInfo.numVertices = N;

        for (int lat = 0; lat <= (geometryInfo.numVertices * 3); ++lat) {
            double theta = lat * M_PI / (geometryInfo.numVertices * 3);
            double sinTheta = sin(theta);
            double cosTheta = cos(theta);

            for (int lon = 0; lon <= (geometryInfo.numVertices * 3); ++lon) {
                double phi = lon * 2 * M_PI / (geometryInfo.numVertices * 3);
                double sinPhi = sin(phi);
                double cosPhi = cos(phi);

                float x = static_cast<float>(cosPhi * sinTheta) / 2;
                float y = static_cast<float>(cosTheta) / 2;
                float z = static_cast<float>(sinPhi * sinTheta) / 2;

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
