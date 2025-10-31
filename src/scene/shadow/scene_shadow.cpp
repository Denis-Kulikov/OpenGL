#include <scene/shadow/scene_shadow.hpp>
#include <scene/scene.hpp>

void SceneShadow::ShadowPass(const Scene* scene) {
    glm::vec3 position = scene->lighting.DataSSBO.pointLights[0].position;
    pointLights.ShadowPass(scene, position);
    directionalLight.ShadowPass(scene, &scene->lighting.DataSSBO.dirLights, glm::vec3(0.0f));
    WindowManager::BindFrameBuffer();
}
    