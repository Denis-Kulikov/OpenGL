#include <game/gameManager.hpp>
#include <object/cube_wire.hpp>

struct GeometryInfo Cube_wire::geometryInfo = {0, 0, 0, 0, 0};

void Cube_wire::initializeGeometry()
{
    std::vector<GLfloat> vertices = {
        -0.5, -0.5, 0.5,
        -0.5, 0.5, 0.5,   -0.5, 0.5, -0.5,  -0.5, 0.5, 0.5, 
        0.5, 0.5, 0.5,    0.5, 0.5, -0.5,    0.5, 0.5, 0.5,
        0.5, -0.5, 0.5,   0.5, -0.5, -0.5,   0.5, -0.5, 0.5,
        -0.5, -0.5, 0.5,

        -0.5, -0.5, -0.5,
        -0.5, 0.5, -0.5,
        0.5, 0.5, -0.5,
        0.5, -0.5, -0.5,
        -0.5, -0.5, -0.5
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

struct GeometryInfo *Cube_wire::GetGeometry()
{
    return &geometryInfo;
}

void Cube_wire::Render(void *RenderData) const 
{
    if (GameManager::render->pipeline.camera == nullptr) {
        std::cout << "Error Render: not found camera" << std::endl;
        return;
    }

    GameManager::render->PushGeometry(&geometryInfo);

    glUseProgram(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(gTextureSamplerLocation, 0);

    glUniform3f(gColorLocation, color.x, color.y, color.z);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &static_cast<Sprite_rdata*>(RenderData)->matrix);

    if (GameManager::render->LineWidth != width) {
        GameManager::render->LineWidth = width;
        glLineWidth(GameManager::render->LineWidth);
    }
    
    glDrawArrays(GL_LINE_STRIP, 0, geometryInfo.numVertices);
}

Cube_wire::Cube_wire(const std::string &_name, const glm::vec3 _color)
    : Line (_name, _color)
{
    gColorLocation = glGetUniformLocation(shader, "gColor");
}

Cube_wire::Cube_wire()
{
    gColorLocation = glGetUniformLocation(shader, "gColor");
}
