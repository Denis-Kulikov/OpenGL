#pragma once
#include <glm/glm.hpp>

struct DirectionalLight {
    DirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity)
        : direction(direction), color(color), intensity(intensity)
    {}

    DirectionalLight()
        : direction(glm::vec3(0)), color(glm::vec3(0)), intensity(0)
    {}

    glm::vec3 direction;
    float padding1;
    glm::vec3 color;
    float intensity;
};
