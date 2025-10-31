#pragma once
#include <glm/glm.hpp>

struct PointLight {
    #define POINT_LIGHT_NUM 128

    PointLight(const glm::vec3 position, float radius, const glm::vec3& color, float intensity, int shadowIndex) 
        : position(position), radius(radius), color(color), intensity(intensity), shadowIndex(shadowIndex)
    {}

    PointLight()
        : PointLight(glm::vec3(0.0f), 0.0f, glm::vec3(0.0f), 0.0f, -1)
    {}

    glm::vec3 position;
    float radius;
    glm::vec3 color;
    float intensity;
    int shadowIndex;
    float padding1;
    float padding2;
    float padding3;
};
