#include <game/gameManager.hpp>
#include <entities/templates/playable/unit.hpp>
#include <entities/templates/mobs/bullet.hpp>


Scene *createScene(int id)
{
    Unit::Initialize();
    Bullet::Initialize();

    auto *scene = new Scene(std::string("Main scene"), id);

    scene->Time.Update();
    Actor::PushTime(scene->Time.GetCurrentTime());

    Sprite* mySprite = new Sprite(std::string("Floor"), "shaders/sprite_fs.glsl", "shaders/sprite_repeat_vs.glsl", "img/chess_frame.jpg");
    mySprite->Magnification.second = mySprite->Magnification.first = 5;
    objectTransform transformFloor;
    transformFloor.SetWorldPos(0, 0.0, -0.02);
    transformFloor.SetRotate(0, 0, 0);
    transformFloor.SetScale(20, 20, 0);
    Component* component = new Component(transformFloor, mySprite);
    scene->pushObject(*component);

    scene->player->Teleport(GameManager::client.spawn);
    scene->player->createCamera(GameManager::width, GameManager::height);
    GameManager::PushPlayer(scene->player);
    GameManager::PushCamera(scene->player->GetCamera());
    GameManager::render->SetCamera(scene->player->GetCamera());
    GameManager::render->PushGeometry(mySprite->GetGeometry());

    std::cout << "Size = " << scene->players.size() << std::endl;
    for (auto& it : scene->players) {
        if (it.second.id == scene->player->id) continue;
        it.second.Teleport(Vector3<float>(100000, 100000, -0.5f));
    }

    return scene;
}