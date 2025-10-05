#pragma once
#include <glm/glm.hpp>

struct Light {
    int type;              // 0 = directional, 1 = point, 2 = spot
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 direction;
    alignas(16) glm::vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
    float innerCutoff;
    float outerCutoff;
};
