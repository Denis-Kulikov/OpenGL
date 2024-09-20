#pragma once
#include "../object/objectTransform.hpp"


namespace light
{
    class GlobalIllumination
    {
        glm::vec3 Color;
        glm::vec3 Direction;
        float AmbientIntensity;
        float DiffuseIntensity;
    };
}
