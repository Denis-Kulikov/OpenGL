#pragma once
#include "objectTransform.hpp"

class sprite 
{
public:
    sprite(const std::string &_name, const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath);
    sprite(const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath);
    static void initializeGeometry();

    std::string      name;
    objectTransform  trans;
    Vector3<GLfloat> color = Vector3<GLfloat>(1.0, 1.0, 1.0);

    GLuint texture = 0;
    GLuint shader  = 0;
    
    GLuint gWorldLocation;
    GLuint gScaleLocation;
    GLuint gRotateLocation;
    GLuint gColorLocation;

    GLuint gTextureSamplerLocation;

    static GLuint VAO;
    static GLuint VBO;
    static GLuint EBO;

    static GLint numVertices;
    static GLint numIndices;
    
protected:
    GLuint loadShader(const char *shaderPath, GLuint type);
    void loadTexures(const char *texturePath);
    void compileShaders(const char *FS, const char *VS);

};
