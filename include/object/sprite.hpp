#pragma once
#include "objectTransform.hpp"
#include "../object/renderableObject.hpp"

struct GeometryInfo {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    GLint numVertices = 0;
    GLint numIndices = 0;
};

class Sprite : public RenderableObject
{
public:
    Sprite(const std::string &_name, const char *FS, const char *VS, const char *texturePath);
    static void initializeGeometry();
    virtual struct GeometryInfo *GetGeometry();

    struct Sprite_rdata {
        Matrix4f matrix;
    };

    virtual void Render(void *RenderData) const override;

    std::string name;
    glm::vec3 Scale;
    glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);

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
