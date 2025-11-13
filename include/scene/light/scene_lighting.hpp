#pragma once
#include <array>
#include <glm/glm.hpp>
#include "point_light.hpp"
#include "directional_light.hpp"

class SceneLighting {
public:    
    SceneLighting();

    struct LightsSSBO {
        LightsSSBO(int numPointLights,
                   int numDirectionalLights,
                   int numSpotLights,
                   std::array<PointLight, POINT_LIGHT_NUM> pointLights,
                   DirectionalLight dirLights
        )
        :   numPointLights(numPointLights),
            numDirectionalLights(numDirectionalLights),
            numSpotLights(numSpotLights),
            pointLights(pointLights),
            dirLights(dirLights)
        {}

        int numPointLights;
        int numSpotLights;
        int numDirectionalLights;
        int padding1;

        std::array<PointLight, POINT_LIGHT_NUM> pointLights;
        // SpotLight spotLights[32];
        DirectionalLight dirLights;
    };

    void Initialize();

    LightsSSBO DataSSBO;
};
