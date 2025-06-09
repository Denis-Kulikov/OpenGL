#include <managers/global_state.hpp> 
#include <managers/render_manager.hpp> 
#include <managers/time_manager.hpp> 
#include <managers/window_manager.hpp> 

#include <object/scene.hpp>
#include <entities/templates/decor/grass.hpp>
#include <entities/templates/decor/brick_sphere.hpp>
#include <entities/templates/decor/wooden_box.hpp>
#include <entities/templates/decor/skybox.hpp>
#include <entities/templates/mobs/female.hpp>
#include <entities/templates/playable/Ghost.hpp>

#include <object/component/template/skeletal_mesh.hpp>


void draw(Scene *scene) {
    TimeManager::Update();
    RenderManager::UpdateCamera();
    RenderManager::render.UpdatePV_Perspective();
    RenderManager::render.drawSkybox(*scene->skybox);

    GlobalState::GetPlayer()->MoveForward();

    for (auto &it : scene->actors)
        it->Render();

    WindowManager::SwapBuffer();
}

Scene *createScene()
{
    Ghost::Initialize();
    Grass::Initialize();
    Skybox::Initialize();
    WoodenBox::Initialize();
    Female::Initialize();
    BrickSphere::Initialize();

    auto *scene = new Scene();

    Actor *character = new Ghost();
    scene->pushObject(character);

    auto grass = new Grass();
    grass->Teleport(glm::vec3(-1, -1, 5));
    grass->rootComponent->SetScale(glm::vec3(3));
    grass->rootComponent->SetRotation(glm::vec3(90, 0, 0));
    scene->pushObject(grass);

    auto cube = new WoodenBox();
    cube->Teleport(glm::vec3(0, -0.5, 4 + 2));
    scene->pushObject(cube);

    auto sphere = new BrickSphere();
    sphere->Teleport(glm::vec3(-2, -0.5, 3 + 1));
    sphere->rootComponent->SetScale(glm::vec3(0.5));
    scene->pushObject(sphere);

    //
    auto sphere_ = new BrickSphere();
    auto sph = static_cast<ComponentShape*>(sphere_->rootComponent);
    sphere_->rootComponent->SetScale(glm::vec3(15));

    auto female = new Female();
    sphere_->rootComponent->SetScale(glm::vec3(100 / 5));
    female->rootComponent->AddChild(sphere_->rootComponent);
    // auto sm = static_cast<ComponentSkeletalMesh*>(female->rootComponent);
    // sm->AttachToBone(sm->mesh->skeleton.BoneMap.at("_rootJoint"), sphere_->rootComponent);

    // female->rootComponent->SetRotation(glm::vec3(-90, 45, 0));
    female->rootComponent->SetPosition(glm::vec3(0, -1, 4.2));
    female->SetScale(glm::vec3(0.01));

    scene->pushObject(female);

    scene->skybox = new Skybox();

    GlobalState::SetPlayer(character);

    return scene;
}


int main(int argc, char** argv)
{
    const GLfloat width = 1600, height = 900;

    WindowManager::Initialize(width, height);
    RenderManager::Initialize(70.0f, width, height, 0.1f, 1000.0f);
    TimeManager::Initialize();

    Scene *scene(createScene());

    while (GlobalState::isAppRunning) {
        draw(scene);
    }

    WindowManager::Dispose();

    return 0;
}
