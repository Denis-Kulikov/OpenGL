#pragma once
#include "lib-project/lib.hpp"
#include "math/Vector3.hpp"
#include <stb_image.h>

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    ~Skybox();

    void Load();
    void Render();

private:
    GLuint skyboxVAO, skyboxVBO;
    GLuint cubemapTexture;
    std::vector<std::string> faces;
};
