#pragma once
#include "glfw.hpp"

class sprite 
{
public:
    sprite(const std::string &_name, const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath);
    sprite(const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath);

    std::string      name;
    objectTransform  trans;
    Vector3<GLfloat> color = Vector3<GLfloat>(1.0, 1.0, 1.0);

    GLuint texture = 0;
    GLuint shader  = 0;
    
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    GLint numVertices = 0;
    GLint numIndices  = 0;

    GLuint gWorldLocation;
    // GLuint gScaleLocation;
    // GLuint gRotateLocation;
    GLuint gColorLocation;

    GLuint gTextureSamplerLocation;
    
protected:
    GLuint loadShader(const char *shaderPath, GLuint type);
    void loadTexures(const char *texturePath);
    void compileShaders(const char *FS, const char *VS);

private:
    void initializeGeometry();

};
