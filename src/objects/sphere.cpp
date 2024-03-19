#include <render/glfw.hpp>
#include <object/sphere.hpp>

void sphere::initializeGeometry(const GLint _numVertices)
{
    // if (geometry != nullptr) free(geometry);
    // geometry = static_cast<struct GeometryInfo>(calloc(1, sizeof(struct GeometryInfo)));
    // geometry = static_cast<struct GeometryInfo*>(calloc(1, sizeof(struct GeometryInfo)));
    geometry = new struct GeometryInfo;
    geometry->VAO = 0;
    geometry->VBO = 0;
    geometry->EBO = 0;
    geometry->numVertices = 0;
    geometry->numIndices = 0;

    if (geometry == nullptr) {
        std::cout << "Error sphere::initializeGeometry(): calloc(sizeof(struct geometry)" << std::endl;
        assert(geometry != nullptr);
    }

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    geometry->numVertices = _numVertices;

    for (int lat = 0; lat <= (geometry->numVertices * 3); ++lat) {
        double theta = lat * M_PI / (geometry->numVertices * 3);
        double sinTheta = sin(theta);
        double cosTheta = cos(theta);

        for (int lon = 0; lon <= (geometry->numVertices * 3); ++lon) {
            double phi = lon * 2 * M_PI / (geometry->numVertices * 3);
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

    for (int lat = 0; lat < (geometry->numVertices * 3); ++lat) {
        for (int lon = 0; lon < (geometry->numVertices * 3); ++lon) {
            int current = lat * ((geometry->numVertices * 3) + 1) + lon;
            int next = current + (geometry->numVertices * 3) + 1;

            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }

    geometry->numIndices = static_cast<int>(indices.size());

    glGenVertexArrays(1, &geometry->VAO);
    glBindVertexArray(geometry->VAO);

    glGenBuffers(1, &geometry->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &geometry->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// sphere::sphere(const std::string &_name, const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath, const GLint _numVertices)
sphere::sphere(const std::string &_name, const char *FS, const char *VS, const char *texturePath, const GLint _numVertices)
    : Sprite(_name, FS, VS, texturePath)
{
    initializeGeometry(_numVertices);
}
