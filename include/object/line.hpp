#pragma once
#include "sprite.hpp"

class line : public sprite
{
public:
    line(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> _color);
    line(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> _color, GLfloat _lineWidth);

    void setPoints(const Vector3<GLfloat> &_start, const Vector3<GLfloat> &_end);
    static void initializeGeometry();
    void draw();

    static GLuint VAO;
    static GLuint VBO;
    static GLuint EBO;

    static GLint numVertices;
    static GLint numIndices;

    GLfloat lineWidth;
};
