#pragma once
#include "objectTransform.hpp"

struct GeometryInfo {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    GLint numVertices;
    GLint numIndices;
};

class Sprite
{
public:
    Sprite(const std::string &_name, const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath);
    Sprite(const std::string &_name, const char *FS, const char *VS, const char *texturePath);
    Sprite();
    static void initializeGeometry();

    std::string     name;
    objectTransform trans;
    Vector3<GLfloat> color = Vector3<GLfloat>(1.0, 1.0, 1.0);

    GLuint texture = 0;
    GLuint shader  = 0;
    
    GLuint gWorldLocation;
    GLuint gScaleLocation;
    GLuint gRotateLocation;
    GLuint gColorLocation;

    GLuint gTextureSamplerLocation;

    struct GeometryInfo *geometry = nullptr;

protected:
    GLuint loadShader(const char *shaderPath, GLuint type);
    void loadTexures(const char *texturePath);
    void compileShaders(const char *FS, const char *VS);

    static struct GeometryInfo geometryInfo;
};
