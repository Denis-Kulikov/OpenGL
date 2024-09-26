#pragma once
#include "lib-project/lib.hpp"
#include "../object/renderableObject.hpp"
#include <stb_image.h>

class Terrain : public RenderableObject {
public:
    Terrain(const std::vector<std::string>& faces);
    ~Terrain();

    void Load();
    void Render(void *RenderData) const override;

private:
    GLuint terrainVAO, terrainVBO;
    GLuint terrainTexture;
};
