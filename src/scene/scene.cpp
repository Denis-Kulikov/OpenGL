#include <scene/scene.hpp>

Scene::Scene() {
    lighting.Initialize();
    RenderManager::bufferManager.UpdateLightsSSBO(lighting);
}

Scene::~Scene() {
    for (auto& it : actors) delete it;
};

void Scene::pushObject(Actor* actor) { actors.push_back(actor); };

