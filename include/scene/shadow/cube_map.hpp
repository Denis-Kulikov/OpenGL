#pragma once
#include "shadow_map.hpp"
#include <glm/glm.hpp>

class Scene;

class ShadowCubeMap : public ShadowMap {
public:
    #define POINT_LIGHT_SHADOW_NUM 4

    ShadowCubeMap(GLsizei resolution = 1024);
    
    void ShadowPass(const Scene* scene, const glm::vec3 lightPos);

private:
    float nearPlane = 0.1f;
    float farPlane = 128.0f;
};