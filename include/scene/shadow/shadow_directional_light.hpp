#pragma once
#include "shadow_map.hpp"
#include "../light/directional_light.hpp"

class Scene;

class ShadowDirectionalLight : public ShadowMap {
public:
    ShadowDirectionalLight(GLsizei SHADOW_WIDTH = 2048, GLsizei SHADOW_HEIGHT = 2048);
    void ShadowPass(const Scene* scene, const DirectionalLight* light, const glm::vec3& offset);

    glm::mat4 lightSpaceMatrix;
};
