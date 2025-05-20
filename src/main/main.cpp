#include <managers/global_state.hpp> 
#include <managers/render_manager.hpp> 
#include <managers/time_manager.hpp> 
#include <managers/window_manager.hpp> 

#include <object/scene.hpp>
#include <object/cube.hpp>
#include <object/sphere.hpp>
#include <object/sphere_wire.hpp>
#include <entities/templates/playable/Ghost.hpp>

#include <stb_image.h>
#include <stb_image_write.h>


void draw(Scene *scene) {
    TimeManager::Update();
    RenderManager::UpdateCamera();
    RenderManager::render.UpdatePV_Perspective();
    RenderManager::render.drawSkybox(*scene->skybox);

    GlobalState::GetPlayer()->MoveForward();
    GlobalState::GetPlayer()->rootComponent->UpdateMatrixTree();

    for (auto &it : scene->shapes)
        it->Render();

    WindowManager::SwapBuffer();
}

Scene *createScene()
{
    Cube::initializeGeometry();
    Sprite::initializeGeometry();

    auto *scene = new Scene();

    Pawn *character = new Ghost();
    scene->pushObject(character);

    scene->skybox = new Cube("img/skybox.png");

    auto cube = new Cube("img/box.jpg");
    auto cube_shape = new Shape(cube);
    cube_shape->GetTransform()->SetPosition(glm::vec3(0, 0, 2));
    scene->pushObject(cube_shape);

    GlobalState::SetPlayer(character);
    RenderManager::render.pipeline.camera.OwnerTransformPtr = character->rootComponent->GetTransform();

    return scene;
}


int main(int argc, char** argv)
{
    const GLfloat width = 1600, height = 900;

    RenderManager::Initialize(70.0f, width, height, 0.5f, 7000.0f);
    TimeManager::Initialize();
    WindowManager::Initialize(width, height);

    Scene *scene(createScene());

    while (GlobalState::isAppRunning) {
        draw(scene);
    }

    WindowManager::Dispose();

    return 0;
}
