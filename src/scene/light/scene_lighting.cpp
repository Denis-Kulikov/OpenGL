#include <scene/light/scene_lighting.hpp>

SceneLighting::SceneLighting()
    : DataSSBO(0, 0, 0, std::array<PointLight, POINT_LIGHT_NUM>(), std::array<DirectionalLight, 16>())
{}

void SceneLighting::Initialize() {
    DataSSBO.numPointLights = 1;
    DataSSBO.pointLights[0] = PointLight(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f), 100.0f);
    DataSSBO.dirLights[0] = DirectionalLight(glm::vec4(0.0f, -1.0f, 0.0f, 0.0f), glm::vec3(1.0f), 0.5f);
}
