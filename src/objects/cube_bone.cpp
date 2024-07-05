#include <render/glfw.hpp>
#include <object/cube_bone.hpp>
#include <lib-project/try.hpp>

struct GeometryInfo cube_bone::geometryInfo = {0, 0, 0, 0, 0};

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

struct GeometryInfo *cube_bone::GetGeometry()
{
    return &geometryInfo;
}


cube_bone::cube_bone(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> _color)
    : line (_name, _trans, _color)
{
}
