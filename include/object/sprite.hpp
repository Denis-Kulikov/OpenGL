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
    Sprite(const std::string &_name, const char *FS, const char *VS, const char *texturePath);
    static void initializeGeometry();
    virtual struct GeometryInfo *GetGeometry();

    std::string name;
    Vector3<GLfloat> Scale;
    Vector3<GLfloat> color = Vector3<GLfloat>(1.0, 1.0, 1.0);

    GLuint texture = 0;
    GLuint shader  = 0;
    
    GLuint gWorldLocation;
    GLuint gColorLocation;
    GLuint gTextureSamplerLocation;

protected:
    void compileShaders(const char *FS, const char *VS);
    GLuint loadShader(const char *shaderPath, GLuint type);
    void loadTextures(const char *texturePath);

    inline static struct GeometryInfo geometryInfo = {0, 0, 0, 0, 0};

private:
    inline static std::map<std::string, std::array<GLuint, 4>> shadersMap;
    inline static std::map<std::string, GLuint> texturesMap;
};
