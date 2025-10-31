#pragma once
#include "shadow_map.hpp"

class Scene;

class ShadowPointLight : public ShadowMap {
public:
    #define SHADOW_POINT_LIGHT__NUM 4

    ShadowPointLight(GLsizei SHADOW_SIZE = 1024);
    
    void ShadowPass(const Scene* scene, const glm::vec3 lightPos);
    glm::vec3 GetPosition() {return lightPosition;}

    glm::vec3 lightPosition;
};