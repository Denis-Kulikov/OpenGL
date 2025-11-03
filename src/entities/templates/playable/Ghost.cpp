#include <entities/templates/playable/Ghost.hpp>
#include <object/component/template/point.hpp>
#include <object/component/template/camera.hpp>
#include <managers/render/render.hpp> 
#include <managers/window/window_manager.hpp> 
#include <managers/window/window.hpp> 

Ghost::Ghost() {
    ACTOR_SET_NAME;

    Transform *transformPoint = new Transform();
    ComponentPoint *point = CreateComponent<ComponentPoint>(transformPoint);

    Transform *transformCamera = new Transform();
    ComponentCamera *camera = CreateComponent<ComponentCamera>(transformCamera);
    camera->camera.SetPerspectiveProj(90.0f, WindowManager::curWindow->GetWidth(), WindowManager::curWindow->GetHeight(), 0.5f, 512.0f);
    WindowManager::curWindow->GetScene()->SetCamera(camera);

    point->AddChild(camera);

    rootComponent = point;
}

Ghost::~Ghost() {}

void Ghost::Initialize() {
    originalName = "Ghost";
}

ACTOR_BASE_CPP(Ghost);
