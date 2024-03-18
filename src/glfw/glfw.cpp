#include <render/glfw.hpp>
#include <render/render.hpp>
#include <object/scene.hpp>
#include <object/sphere.hpp>
#include <object/line.hpp>
#include <object/cube_bone.hpp>
#include <game/gameManager.hpp>
#include <entities/actor.hpp>
#include <entities/pawn.hpp>
#include <entities/character.hpp>
#include <entities/templates/playable/Wilson.hpp>
#include <entities/templates/playable/spider.hpp>
#include <omp.h>

#include <chrono>
#include <ctime>
#include <random>
#include <thread>

// Растеризация. Проекция перспективы. Скелетная анимация 2D моделей.

std::map<std::string, Sprite> Spider::Sprites;
size_t Spider::skeletSize = 0;
Bone Spider::skelet;

#define SPIDER_NUM 4

Wilson *character = nullptr;
Spider *spider[SPIDER_NUM] = {nullptr};

time_t prev = time(0);
int frame = 0;

std::chrono::milliseconds totalTime(0);

float randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

Vector3<GLfloat> generateRandomPoint() {
    float MIN_X = -4.0, MAX_X = 4.0;
    float MIN_Y = -4.0, MAX_Y = 4.0;
    float MIN_Z = 0.0, MAX_Z = 1.0;
    float x = randomFloat(MIN_X, MAX_X);
    float y = randomFloat(MIN_Y, MAX_Y);
    float z = randomFloat(MIN_Z, MAX_Z);
    return Vector3<GLfloat>(x, y, z);
}

bool RenderSceneCB(Render *render, Scene *scene)
{
    static const int targetFPS = 60;
    static const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    static std::chrono::steady_clock::time_point frameStart;
    std::chrono::steady_clock::time_point frameEnd = std::chrono::steady_clock::now();

    std::this_thread::sleep_until(frameStart + frameDuration);
    frameStart = std::chrono::steady_clock::now();


    for (std::vector<Component>::iterator it = scene->getIterator(); it != scene->component.end(); it++)
        GameManager::render->drawObject(&it->transform, it->sprite);

    character->MoveForward();
    std::vector<Component*> ActorComponents = character->getActorComponents();

    character->UpdateCameraPos();

    for (auto it : ActorComponents) {
        if (it->sprite == nullptr) continue;
        GameManager::render->drawObject(&it->transform, it->sprite);
    }

    for (int i = 0; i < SPIDER_NUM; i++) {
        spider[i]->MoveTowards(character, 0.006);
        ActorComponents = spider[i]->getActorComponents();
        for (auto it : ActorComponents) {
            if (it->sprite == nullptr) continue;
            GameManager::render->drawObject(&it->transform, it->sprite);
        }
    }

    
    frame++;
    if ((time(0) - prev) > 3) {
        std::cout << "FPS: " << frame / 3 << std::endl;
        // std::cout << "Total time: " << totalTime.count() << " milliseconds" << std::endl;
        // totalTime = std::chrono::milliseconds(0);
        prev = time(0);
        frame = 0;
    }

    // std::cout << character->direction.x << '\t' << character->direction.y << '\t' << character->direction.z << '\t' << std::endl;

    // for (int i = 0; i < 2; i++) {
        // if (character->direction[i]) std::cout << i << "\t" << character->direction[i] << std::endl;
    // }

    return GameManager::IsEnd;
}

Scene *createScene()
{
    auto *scene = new Scene(std::string("Main scene"));

    Sprite *mySprite = new Sprite(std::string("Grass"), "shaders/Sprite_fs.glsl", "shaders/Sprite_vs.glsl", "img/grass.png");
    objectTransform transformGrass;
    transformGrass.Move(0, -3, -2);
    transformGrass.SetRotate(90, 0, 0);
    transformGrass.SetScale(10, 10, 0);
    Component component(transformGrass, mySprite);
    scene->pushObject(component);

    Wilson::Initialize();
    Spider::Initialize();

    character = new Wilson();
    character->createCamera(GameManager::width, GameManager::height);

    character->updateAnimation("stand");

    for (int i = 0; i < SPIDER_NUM; i ++) {
        spider[i] = new Spider();
        spider[i]->Teleport(generateRandomPoint());
        spider[i]->updateAnimation("stand");
    }

    GameManager::PushPlayer(reinterpret_cast<Player*>(character)); // ***
    GameManager::PushCamera(character->GetCamera());
    GameManager::render->SetCamera(character->GetCamera());

    return scene;
}
