#include <glfw.hpp>
#include <sphere.hpp>
#include <try.hpp>

void sphere::initializeGeometry()
{
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    for (int lat = 0; lat <= (numVertices * 3); ++lat) {
        double theta = lat * M_PI / (numVertices * 3);
        double sinTheta = sin(theta);
        double cosTheta = cos(theta);

        for (int lon = 0; lon <= (numVertices * 3); ++lon) {
            double phi = lon * 2 * M_PI / (numVertices * 3);
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

    for (int lat = 0; lat < (numVertices * 3); ++lat) {
        for (int lon = 0; lon < (numVertices * 3); ++lon) {
            int current = lat * ((numVertices * 3) + 1) + lon;
            int next = current + (numVertices * 3) + 1;

            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }

    numIndices = static_cast<int>(indices.size());

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

sphere::sphere(const std::string &_name, const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath, const GLint _numVertices)
    : sprite(_name, _trans, FS, VS, texturePath)
{
    numVertices = _numVertices;
    initializeGeometry();
}
