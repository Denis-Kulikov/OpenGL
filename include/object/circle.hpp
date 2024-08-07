#pragma once
#include "sprite.hpp"

class circle : public Sprite
{
public:
    circle(const std::string &_name, const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath, const GLint _numVertices);

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    GLint numVertices;
    GLint numIndices;
    
private:
    void initializeGeometry();
};
