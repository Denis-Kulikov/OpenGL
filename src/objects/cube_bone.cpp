#include <glfw.hpp>
#include <cube_bone.hpp>
#include <try.hpp>

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
    : sprite(_trans, "shaders/cube_bone_fs.glsl", "shaders/base_vs.glsl", nullptr)
{
    name = _name;
    color.x = _color.x;
    color.y = _color.y;
    color.z = _color.z;
    initializeGeometry();
}
