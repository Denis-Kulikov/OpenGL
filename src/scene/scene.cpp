#include <scene/scene.hpp>
#include <managers/global.hpp> 

Scene::Scene() {
    lighting.Initialize();
    RenderManager::bufferManager.UpdateLightsSSBO(lighting);
}

Scene::~Scene() {
    for (auto& it : actors) delete it;
};

void Scene::SetCamera(ComponentCamera* camera)
{
    curCamera = camera;
    curCamera->UpdatePerspective();
}

void Scene::UpdateCamera() {
    if (curCamera != nullptr) {
        curCamera->camera.UpdateTarget();
        curCamera->UpdateView();
        curCamera->UpdatePV();
    }
}

void Scene::pushObject(Actor* actor) { actors.push_back(actor); };

void Scene::Update(float deltaTime) {
    for (auto &it : actors) {
        if (it->rootComponent) {
            it->rootComponent->UpdateTree(deltaTime);
        }
    }
}

void Scene::Render() {
    for (auto &it : actors) {
        // std::cout << "Name: " << it->GetName() << std::endl;
        it->Render();
    }
};

void Scene::DrawSkybox() {
    if (curCamera == nullptr || skybox == nullptr) return;

    GLint oldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
    GLint oldDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);
    GLboolean oldDepthMask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &oldDepthMask);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    skybox->rootComponent->SetPosition(curCamera->GetGlobalPosition());

    glm::quat quaternion = glm::quat(glm::radians(glm::vec3(0, 0, 180)));
    if (skybox->rootComponent->GetRotation() != quaternion)
        skybox->rootComponent->SetRotation(quaternion);

    if (skybox->rootComponent->GetScale() != glm::vec3(2))
        skybox->rootComponent->SetScale(glm::vec3(2));
        
    skybox->Render();

    glCullFace(oldCullFaceMode);
    glDepthFunc(oldDepthFunc);
    glDepthMask(oldDepthMask);
}

void Scene::Callback(float deltaTime) {
    // BulletManager::StepSimulation(deltaTime);
    UpdateCamera();
    RenderManager::bufferManager.Update();
    DrawSkybox();

    if (GlobalState::GetPlayer() != nullptr)
        GlobalState::GetPlayer()->MoveForward();

    Update(deltaTime);
    shadow.ShadowPass(this);
    Render();
}
