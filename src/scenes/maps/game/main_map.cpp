#include <game/gameManager.hpp>
#include <game/gameTime.hpp>
#include <entities/templates/playable/Wilson.hpp>
#include <entities/templates/mobs/spider.hpp>
#include <entities/templates/decor/wave.hpp>

#include <chrono>
#include <ctime>
#include <random>
#include <thread>

#define SPIDER_NUM 3
#define WAVE_SUM 5

Wilson *character = nullptr;
// Spider *spider[SPIDER_NUM] = {nullptr};
// Wave *wave[WAVE_SUM] = {nullptr};


float randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

Vector3<GLfloat> generateRandomPoint() {
    float MIN_X = -16.0, MAX_X = 16.0;
    float MIN_Y = -2.0, MAX_Y = 2.0;
    float MIN_Z = -16.0, MAX_Z = 2.0;
    float x = randomFloat(MIN_X, MAX_X);
    float y = randomFloat(MIN_Y, MAX_Y);
    float z = randomFloat(MIN_Z, MAX_Z);
    return Vector3<GLfloat>(x, y, z);
}

Scene *createScene()
{
    auto *scene = new Scene(std::string("Main scene"));

    Sprite *mySprite = new Sprite(std::string("Grass"), "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", "img/grass.png");
    objectTransform transformGrass;
    transformGrass.Move(0, -5, -2);
    transformGrass.SetRotate(90, 0, 0);
    transformGrass.SetScale(10, 10, 0);
    Component *component;
    component = new Component(transformGrass, mySprite);
    // scene->pushObject(*component);

    mySprite = new Sprite(std::string("Floor"), "shaders/sprite_fs.glsl", "shaders/sprite_repeat_vs.glsl", "img/chess_frame.jpg");
    mySprite->Magnification.second = mySprite->Magnification.first = 5;
    objectTransform transformFloor;
    transformFloor.Move(5, -1.0, -5);
    transformFloor.SetRotate(0, 0, 0);
    transformFloor.SetScale(20, 20, 0);
    component = new Component(transformFloor, mySprite);
    scene->pushObject(*component);


    Wilson::Initialize();
    Spider::Initialize();
    Wave::Initialize();

    character = new Wilson();
    character->createCamera(GameManager::width, GameManager::height);
    character->updateAnimation("stand");
    #if MY_ACTOR_TEST
    character->PushRender(GameManager::render);
    #endif

    // for (int i = 0; i < SPIDER_NUM; i++) {
    //     spider[i] = new Spider();
    //     spider[i]->Teleport(generateRandomPoint());
    //     spider[i]->updateAnimation("stand");
    // }


    // for (int i = 0; i < WAVE_SUM; i++) {
    //     wave[i] = new Wave();
    //     wave[i]->updateAnimation("stand");
    //     objectTransform *transform = wave[i]->GetTransform();
    //     transform->Move(generateRandomPoint());
    // }


    GameManager::PushPlayer(character);
    GameManager::PushCamera(character->GetCamera());
    GameManager::render->SetCamera(character->GetCamera());
    GameManager::render->PushGeometry(mySprite->GetGeometry());

    return scene;
}