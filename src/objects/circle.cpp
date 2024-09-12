#include <object/circle.hpp>

void circle::initializeGeometry()
{
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<GLfloat> texCoords;

    assert(numVertices >= 3);

    float deg = 360.0f / numVertices;
    for (size_t i = 0; i < numVertices; ++i) {
        float angle_rad = radians(deg * i);
        float x = cos(angle_rad);
        float y = sin(angle_rad);
        float z = 0.0f;

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);

        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(numVertices);

        texCoords.push_back((x + 1) / 2);
        texCoords.push_back((y + 1) / 2);
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &texture);
    glBindBuffer(GL_ARRAY_BUFFER, texture);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indices.size() - 3) * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glUniform1i(glGetUniformLocation(shader, "textureSampler"), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

circle::circle(const std::string &_name, const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath, const GLint _numVertices)
    : Sprite(_name, FS, VS, nullptr)
{
    numVertices = _numVertices;
    initializeGeometry();
}
