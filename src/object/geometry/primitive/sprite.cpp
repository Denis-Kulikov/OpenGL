#include <managers/render_manager.hpp>
#include <object/geometry/primitive/sprite.hpp>

#include <stb_image.h>
#include <stb_image_resize.h>
#include <GLFW/glfw3.h>


Sprite::Sprite()
{
    SetType(PRIMITIVE);
    // |2|3|
    // |1|4|
    std::vector<GLfloat> vertices = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,  1.0f, 0.0f
    };

    std::vector<GLuint> indices = {
        0, 1, 2,
        0, 2, 3
    };

    numVertices = vertices.size();
    numIndices = indices.size();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}
