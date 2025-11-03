#include <scene/light/scene_lighting.hpp>

SceneLighting::SceneLighting()
    : DataSSBO(0, 0, 0, std::array<PointLight, POINT_LIGHT_NUM>(), DirectionalLight())
{}

void SceneLighting::Initialize() {
    DataSSBO.numPointLights = 1;
    DataSSBO.pointLights[0] = PointLight(glm::vec3(0.0f, 8.0f, 5.0f), 32.0f, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0);

    DataSSBO.numDirectionalLights = 1;
    // DataSSBO.dirLights = DirectionalLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f), 0.125f);
    DataSSBO.dirLights = DirectionalLight(glm::vec3(2.0f, -4.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
}
