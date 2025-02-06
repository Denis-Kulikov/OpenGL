#include <game/gameManager.hpp>
#include <object/line.hpp>

struct GeometryInfo Line::geometryInfo = {0, 0, 0, 0, 0};

void Line::Render(void *RenderData) const {
    GameManager::render.PushGeometry(&geometryInfo);

    glUseProgram(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(gTextureSamplerLocation, 0);

    glUniform3f(gColorLocation, color.x, color.y, color.z);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &static_cast<Sprite_rdata*>(RenderData)->matrix);

    GameManager::render.PushLineWidth(width);
    
    glDrawArrays(GL_LINE_STRIP, 0, geometryInfo.numVertices);
}

objectTransform Line::setPoints(const glm::vec3 &_start, const glm::vec3 &_end)
{
    objectTransform transform;
    glm::vec3 d = _end - _start;
    GLfloat distance = glm::length(d);
    GLfloat angleZ = atan2(d.y, d.x) * 180.0 / M_PI;
    GLfloat angleY = atan2(d.z, glm::length(glm::vec2(d.x, d.y))) * 180.0 / M_PI;

    transform.SetWorldPos(_start.x + d.x, _start.y + d.y, _start.z + d.z);
    transform.SetScale(distance, 0.0, 0.0);
    transform.SetRotate(0.0, angleY, angleZ); 

    return transform;
}


void Line::initializeGeometry() {
    std::vector<GLfloat> vertices = {
        1, 0, 0,
        -1, 0, 0
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

struct GeometryInfo *Line::GetGeometry()
{
    return &geometryInfo;
}

Line::Line(const std::string &_name, const glm::vec3 _color)
    : Sprite(_name, "shaders/wire_fs.glsl", "shaders/base_vs.glsl", "")
{
    color.x = _color.x;
    color.y = _color.y;
    color.z = _color.z;

    gColorLocation = glGetUniformLocation(shader, "gColor");
}

Line::Line()
    : Sprite("", "shaders/wire_fs.glsl", "shaders/base_vs.glsl", "")
{
    gColorLocation = glGetUniformLocation(shader, "gColor");
}
