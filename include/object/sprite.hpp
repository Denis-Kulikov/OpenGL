#pragma once
#include "objectTransform.hpp"

struct GeometryInfo {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    GLint numVertices = 0;
    GLint numIndices = 0;
};

class Sprite
{
public:
    // Sprite(const std::string &_name, const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath);
    Sprite(const std::string &_name, const char *FS, const char *VS, const char *texturePath);
    Sprite();
    static void initializeGeometry();

    std::string name;
    Vector3<GLfloat> Scale;
    Vector3<GLfloat> color = Vector3<GLfloat>(1.0, 1.0, 1.0);

    GLuint texture = 0;
    GLuint shader  = 0;
    
    GLuint gWorldLocation;
    GLuint gScaleLocation;
    GLuint gRotateLocation;
    GLuint gColorLocation;

    GLuint gObjectLocation;
    GLuint gCameraParamsLocation;
    GLuint gPersProjParamsLocation;

    GLuint gTextureSamplerLocation;

    virtual struct GeometryInfo *GetGeometry();

    // struct GeometryInfo *geometry = nullptr; // можно избавится добавив виртуальную функцию GetGeometry() { return &geometryInfo; }

protected:
    GLuint loadShader(const char *shaderPath, GLuint type);
    void loadTextures(const char *texturePath);
    void compileShaders(const char *FS, const char *VS);

    static struct GeometryInfo geometryInfo;
};
