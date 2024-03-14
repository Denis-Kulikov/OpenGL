#include <render/glfw.hpp>
#include <render/render.hpp>
#include <object/scene.hpp>
#include <object/sphere.hpp>
#include <object/line.hpp>
#include <object/cube_bone.hpp>
#include <lib-project/try.hpp>
#include <game/gameManager.hpp>
#include <entities/actor.hpp>
#include <entities/pawn.hpp>
#include <entities/character.hpp>
#include <entities/templates/playable/Wilson.hpp>
#include <entities/templates/playable/spider.hpp>

#include <chrono>
#include <ctime>

#include <random>

// Растеризация. Проекция перспективы. Скелетная анимация 2D моделей.

std::map<std::string, Sprite> Spider::Sprites;
Bone Spider::skelet;

#define SPIDER_NUM 4

Wilson *character = nullptr;
Spider spider[SPIDER_NUM];


time_t prev = time(0);
int frame = 0;


float randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

Vector3<GLfloat> generateRandomPoint() {
    float MIN_X = -4.0, MAX_X = 4.0;
    float MIN_Y = -4.0, MAX_Y = 4.0;
    float MIN_Z = -0.0, MAX_Z = 1.0;
    float x = randomFloat(MIN_X, MAX_X);
    float y = randomFloat(MIN_Y, MAX_Y);
    float z = randomFloat(MIN_Z, MAX_Z);
    return Vector3<GLfloat>(x, y, z);
}


bool RenderSceneCB(Render *render, Scene *scene)
{
    for (std::vector<Component>::iterator it = scene->getIterator(); it != scene->component.end(); it++)
        GameManager::render->drawObject(it->transform, *it->sprite);

    std::vector<Component*> ActorComponents = character->getActorComponents(&character->skelet);

    character->UpdateCameraPos();

    for (auto it : ActorComponents) {
        if (it->sprite == nullptr) continue;
        GameManager::render->drawObject(it->transform, *it->sprite);
    }

    for (int i = 0; i < SPIDER_NUM; i++) {
        spider[i].MoveTowards(character, 0.006);
        ActorComponents = spider[i].getActorComponents(&spider[i].skelet);
        for (auto it : ActorComponents) {
            if (it->sprite == nullptr) continue;
            GameManager::render->drawObject(it->transform, *it->sprite);
        }
    }

    
    frame++;
    if ((time(0) - prev) > 3) {
        std::cout << "FPS: " << frame / 3 << std::endl;
        prev = time(0);
        frame = 0;
    } 

    return GameManager::IsEnd;
}

Scene *createScene()
{
    auto *scene = new Scene(std::string("Main scene"));

    Vector3<GLfloat> whiteColor(1, 1, 1);
    Vector3<GLfloat> redColor(1, 0, 0);
    Sprite *mySprite;
    objectTransform _trans;

    mySprite = new Sprite(std::string("Grass"), _trans, "shaders/Sprite_fs.glsl", "shaders/Sprite_vs.glsl", "img/grass.png");
    objectTransform transformGrass;
    transformGrass.Move(0, -3, 2);
    transformGrass.SetRotate(90, 0, 0);
    transformGrass.SetScale(5, 5, 0);
    Component component(transformGrass, mySprite);
    scene->pushObject(component);

    character = new Wilson();
    GameManager::PushPlayerTransform(character->GetTransform());
    character->createCamera(GameManager::width, GameManager::height);

    Wilson::Initialize();
    Spider::Initialize();

    for (int i = 0; i < SPIDER_NUM; i ++) spider[i].Teleport(generateRandomPoint());


    GameManager::render->SetCamera(character->GetCamera());
    GameManager::PushCamera(character->GetCamera());

    return scene;
}
