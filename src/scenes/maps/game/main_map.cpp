#include <game/gameManager.hpp>
#include <entities/templates/playable/unit.hpp>
#include <entities/templates/mobs/bullet.hpp>

float RandF() {
    float min = -5;
    float max = 5;
    std::random_device rd;  // Источник энтропии
    std::mt19937 gen(rd()); // Генератор случайных чисел с начальным значением от random_device
    std::uniform_real_distribution<float> dis(min, max); // Распределение на интервале [min, max]

    return dis(gen);
}


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
    transformFloor.SetWorldPos(5, -1.0, -0.02);
    transformFloor.SetRotate(0, 0, 0);
    transformFloor.SetScale(20, 20, 0);
    Component* component = new Component(transformFloor, mySprite);
    scene->pushObject(*component);

    scene->player->createCamera(GameManager::width, GameManager::height);
    GameManager::PushPlayer(scene->player);
    GameManager::PushCamera(scene->player->GetCamera());
    GameManager::render->SetCamera(scene->player->GetCamera());
    GameManager::render->PushGeometry(mySprite->GetGeometry());

    std::cout << "Size = " << scene->players.size() << std::endl;
    for (auto& it : scene->players) {
        it.second.Teleport(Vector3<float>(RandF(), RandF(), 0.0f));
    }

    return scene;
}