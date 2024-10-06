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
    Sprite(const std::string &_name, const std::string &FS, const std::string &VS, const std::string &texturePath);
    ~Sprite();
    static void initializeGeometry();
    virtual struct GeometryInfo *GetGeometry();

    struct Sprite_rdata {
        Matrix4f matrix;
    };

    virtual void Render(void *RenderData) const override;

    std::string name;
    glm::vec3 Scale;

    Shader shader;
    GLuint texture = 0;
    
    GLuint gWorldLocation = 0;
    GLuint gTextureSamplerLocation = 0;

protected:
    void loadTextures(const std::string &texturePath);

    inline static struct GeometryInfo geometryInfo = {0, 0, 0, 0, 0};

private:
    inline static std::map<std::string, GLuint> texturesMap;
};
