#include <entities/templates/playable/Ghost.hpp>
#include <object/component/template/point.hpp>
#include <object/component/template/camera.hpp>
#include <managers/render_manager.hpp> 

Ghost::Ghost() {
    std::cout << name << std::endl;
    Transform *transformPoint = new Transform();
    ComponentPoint *point = CreateComponent<ComponentPoint>(transformPoint);

    Transform *transformCamera = new Transform();
    ComponentCamera *camera = CreateComponent<ComponentCamera>(transformCamera);
    camera->camera.PersProj = RenderManager::PersProj;
    RenderManager::SetCamera(camera);

    point->AddChild(camera);

    rootComponent = point;
}

Ghost::~Ghost() {}

void Ghost::Initialize() {
    name = "Ghost";
}

std::string Ghost::GetName() const {
    return name;
}
