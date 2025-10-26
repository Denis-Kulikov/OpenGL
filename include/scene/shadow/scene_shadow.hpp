#pragma once
#include "cube_map.hpp"
#include <managers/window/window.hpp>

class Scene;

class SceneShadow {
public:
    void ShadowPass(const Scene* scene);
    
    ShadowCubeMap cubeMap;
};
