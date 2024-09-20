#include <object/line.hpp>

struct GeometryInfo line::geometryInfo = {0, 0, 0, 0, 0};

void line::setPoints(const glm::vec3 &_start, const glm::vec3 &_end)
{
    glm::vec3 d(_end.x - _start.x, _end.y - _start.y, _end.z - _start.z);
    this->trans.SetWorldPos(_start.x + d.x / 2, _start.y + d.y / 2, _start.z + d.z / 2);

    GLfloat distance = sqrt(pow(d.x, 2) + pow(d.y, 2) + pow(d.z, 2));
    this->trans.SetScale(distance, trans.GetScale().y, 0.0);

    GLfloat angleZ = atan2(d.y, d.x) * 180.0 / M_PI;
    GLfloat angleY = atan2(d.z, sqrt(d.x * d.x + d.y * d.y)) * 180.0 / M_PI;

    this->trans.SetRotate(0.0, angleY, angleZ); 
}


void line::initializeGeometry() {
    std::vector<GLfloat> vertices = {
        -0.5, 0, 0,
         0.5, 0, 0
    };

    geometryInfo.numVertices = 2;

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

line::line(const std::string &_name, const objectTransform &_trans, const glm::vec3 _color)
    : Sprite(_name, "shaders/bone_fs.glsl", "shaders/base_vs.glsl", nullptr)
{
    color.x = _color.x;
    color.y = _color.y;
    color.z = _color.z;

    trans = _trans;
}

struct GeometryInfo *line::GetGeometry()
{
    return &geometryInfo;
}

line::line(const std::string &_name, const glm::vec3 _color)
    : line(_name, objectTransform(), _color)
{
}