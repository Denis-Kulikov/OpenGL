#include <entities/templates/playable/Ghost.hpp>
#include <entities/templates/mobs/Female.hpp>
#include <object/scene.hpp>
#include <object/sphere.hpp>
#include <object/sphere_wire.hpp>
#include <object/cube.hpp>
#include <game/gameManager.hpp>

Scene *CreateScene()
{
    Cube::initializeGeometry();
    Sprite::initializeGeometry();

    auto *scene = new Scene();

    Ghost *character = new Ghost();
    scene->pushObject(character);

    scene->skybox = new Cube("img/skybox.png");

    auto cube = new Cube("img/box.jpg");
    auto cube_shape = new Shape(cube);
    cube_shape->GetTransform()->SetWorldPos(glm::vec3(0, 0, 2));

    scene->pushObject(cube_shape);

    GameManager::PushPlayer(character);
    GameManager::render.pipeline.camera.OwnerTransformPtr = character->GetTransform();

    return scene;
}
