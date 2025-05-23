#include <managers/global_state.hpp> 
#include <managers/render_manager.hpp> 
#include <managers/time_manager.hpp> 
#include <managers/window_manager.hpp> 

#include <object/scene.hpp>
#include <object/primitive/cube.hpp>
#include <entities/templates/decor/wooden_box.hpp>
#include <entities/templates/decor/skybox.hpp>
#include <entities/templates/mobs/female.hpp>
#include <entities/templates/playable/Ghost.hpp>

#include <stb_image.h>
#include <stb_image_write.h>


void draw(Scene *scene) {
    TimeManager::Update();
    RenderManager::UpdateCamera();
    RenderManager::render.UpdatePV_Perspective();
    RenderManager::render.drawSkybox(*scene->skybox);

    GlobalState::GetPlayer()->MoveForward();

    // std::cout << GlobalState::GetPlayer()->rootComponent->children[0]->GetPosition().x << " "
    //     <<GlobalState::GetPlayer()->rootComponent->children[0]->GetPosition().y << " "
    //     <<GlobalState::GetPlayer()->rootComponent->children[0]->GetPosition().z 
    //     << std::endl;

    for (auto &it : scene->actors)
        it->Render();

    WindowManager::SwapBuffer();
}

Scene *createScene()
{
    Skybox::Initialize();
    WoodenBox::Initialize();
    Female::Initialize();

    auto *scene = new Scene();

    Actor *character = new Ghost();
    scene->pushObject(character);

    auto cube = new WoodenBox();
    cube->rootComponent->SetPosition(glm::vec3(0, 0, 2));
    // cube->rootComponent->SetRotation(glm::vec3(0, 0, 45));
    scene->pushObject(cube);

    // auto cube2 = new WoodenBox();
    // cube2->rootComponent->SetPosition(glm::vec3(0, 0, 2));
    // scene->pushObject(cube2);

    auto female = new Female();
    // cube2->rootComponent->SetPosition(glm::vec3(0, 0, 2));
    // cube2->rootComponent->Rotate(glm::vec3(0, 0, 45));
    female->rootComponent->SetPosition(glm::vec3(2, 0, 1));
    female->rootComponent->SetScale(glm::vec3(0.01));
    female->rootComponent->SetScale(glm::vec3(1));
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
