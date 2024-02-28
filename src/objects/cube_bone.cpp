#include <render/glfw.hpp>
#include <object/cube_bone.hpp>
#include <lib-project/try.hpp>

GLuint cube_bone::VAO = 0;
GLuint cube_bone::VBO = 0;
GLuint cube_bone::EBO = 0;
GLint cube_bone::numVertices = 0;
GLint cube_bone::numIndices = 0;

void cube_bone::initializeGeometry()
{
    std::vector<GLfloat> vertices = {
        -1, -1, 1,
        -1, 1, 1,   -1, 1, -1,  -1, 1, 1, 
        1, 1, 1,    1, 1, -1,    1, 1, 1,
        1, -1, 1,   1, -1, -1,   1, -1, 1,
        -1, -1, 1,

        -1, -1, -1,
        -1, 1, -1,
        1, 1, -1,
        1, -1, -1,
        -1, -1, -1
    };

    numVertices = static_cast<int>(vertices.size()) / 3;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

cube_bone::cube_bone(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> _color)
    : line (_name, _trans, _color) {}

cube_bone::cube_bone(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> _color, GLfloat _lineWidth)
    : line (_name, _trans, _color, _lineWidth) {}
