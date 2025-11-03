#pragma once
#include "shadow_directional_light.hpp"
#include "shadow_point_light.hpp"
#include <managers/window/window_manager.hpp>
#include <array>

class Scene;

class SceneShadow {
public:
    void ShadowPass(const Scene* scene);
    
    ShadowPointLight pointLights;
    ShadowDirectionalLight directionalLight;
};
