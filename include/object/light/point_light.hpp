#pragma once
#include <glm/glm.hpp>

struct PointLight {
    PointLight(const glm::vec4 position, const glm::vec4& color, const float radius) 
        : position(position), color(color), radius(radius)
    {}

    PointLight()
        : PointLight(glm::vec4(0.0f), glm::vec4(0.0f), 0.0f)
    {}

    glm::vec4 position;
    glm::vec4 color;
    float radius;
    float padding1;
    float padding2;
    float padding3;
};
