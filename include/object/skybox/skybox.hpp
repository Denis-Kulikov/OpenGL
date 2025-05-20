#pragma once
#include "../renderableObject.hpp"
#include "../objectTransform.hpp"
#include "math/Vector3.hpp"
#include "math/Matrix4f.hpp"
#include <stb_image.h>

class Skybox : public RenderableObject
{
public:
    Skybox(const std::vector<std::string>& faces);
    ~Skybox();

    struct SkyboxData {
        Matrix4f &mat4x4;
    };

    void Load();
    virtual void Render(void *RenderData) const;
    void Render();

private:
    GLuint skyboxVAO, skyboxVBO;
    GLuint cubemapTexture;
    std::vector<std::string> faces;
};
