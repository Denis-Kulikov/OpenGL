#include <scene/shadow/scene_shadow.hpp>
#include <scene/scene.hpp>

void SceneShadow::ShadowPass(const Scene* scene) {
    glm::vec3 position = scene->lighting.DataSSBO.pointLights[0].position;
    cubeMap.ShadowPass(scene, position);
    WindowManager::BindFrameBuffer();
}
    