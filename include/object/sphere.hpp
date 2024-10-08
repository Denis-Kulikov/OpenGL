#pragma once
#include "sprite.hpp"

template<std::size_t N>
class Sphere : public Sprite
{
public:
    Sphere(const std::string &_name, const char *FS, const char *VS, const char *texturePath)
        : Sprite(_name, FS, VS, texturePath) {}

    Sphere()
        : Sphere(std::string("None"), "shaders/sphere_fs.glsl", "shaders/sphere_vs.glsl", nullptr) {}

    struct GeometryInfo *GetGeometry() override {
        return &geometryInfo;
    }

    static void initializeGeometry() {
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;
        geometryInfo.numVertices = N;

        for (int lat = 0; lat <= (geometryInfo.numVertices * 3); ++lat) {
            double theta = lat * M_PI / (geometryInfo.numVertices * 3);
            double sinTheta = sin(theta);
            double cosTheta = cos(theta);

            for (int lon = 0; lon <= (geometryInfo.numVertices * 3); ++lon) {
                double phi = lon * 2 * M_PI / (geometryInfo.numVertices * 3);
                double sinPhi = sin(phi);
                double cosPhi = cos(phi);

                float x = static_cast<float>(cosPhi * sinTheta);
                float y = static_cast<float>(cosTheta);
                float z = static_cast<float>(sinPhi * sinTheta);

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }

        for (int lat = 0; lat < (geometryInfo.numVertices * 3); ++lat) {
            for (int lon = 0; lon < (geometryInfo.numVertices * 3); ++lon) {
                int current = lat * ((geometryInfo.numVertices * 3) + 1) + lon;
                int next = current + (geometryInfo.numVertices * 3) + 1;

                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }

        geometryInfo.numIndices = static_cast<int>(indices.size());

        glGenVertexArrays(1, &geometryInfo.VAO);
        glBindVertexArray(geometryInfo.VAO);

        glGenBuffers(1, &geometryInfo.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, geometryInfo.VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &geometryInfo.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometryInfo.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

private:
    inline static struct GeometryInfo geometryInfo = {0, 0, 0, 0, 0};
};
