#include <render/glfw.hpp>
#include <object/line.hpp>
#include <lib-project/try.hpp>

GLuint line::VAO = 0;
GLuint line::VBO = 0;
GLuint line::EBO = 0;
GLint line::numVertices = 0;
GLint line::numIndices = 0;


// this->trans.SetScale(_start.Distance(static_cast<Vector3<GLfloat>>(_end)), 0.0, 0.0);
void line::setPoints(const Vector3<GLfloat> &_start, const Vector3<GLfloat> &_end)
{
    Vector3<GLfloat> d(_end.x - _start.x, _end.y - _start.y, _end.z - _start.z);
    this->trans.SetWorldPos(_start.x + d.x / 2, _start.y + d.y / 2, _start.z + d.z / 2);

    GLfloat distance = sqrt(pow(d.x, 2) + pow(d.y, 2) + pow(d.z, 2));
    this->trans.SetScale(distance, 0.0, 0.0);

    GLfloat angleZ = atan2(d.y, d.x) * 180.0 / M_PI;
    GLfloat angleY = atan2(d.z, sqrt(d.x * d.x + d.y * d.y)) * 180.0 / M_PI;

    std::cout << "setPoints" << std::endl;

    this->trans.SetRotate(0.0, angleY, angleZ); 
}


void line::initializeGeometry() {
    std::vector<GLfloat> vertices = {
        -0.5, 0, 0,
         0.5, 0, 0
    };

    numVertices = 2;

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

line::line(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> _color)
    : sprite(_name, _trans, "shaders/bone_fs.glsl", "shaders/base_vs.glsl", nullptr), lineWidth(1.0)
{
    color.x = _color.x;
    color.y = _color.y;
    color.z = _color.z;
}


line::line(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> _color, GLfloat _lineWidth)
    : sprite(_name, _trans, "shaders/bone_fs.glsl", "shaders/base_vs.glsl", nullptr), lineWidth(_lineWidth)
{
    color.x = _color.x;
    color.y = _color.y;
    color.z = _color.z;
}
