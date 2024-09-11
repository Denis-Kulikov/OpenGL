#include <game/gameManager.hpp>
#include <entities/templates/playable/Ghost.hpp>
#include <entities/templates/playable/Wilson.hpp>
#include <entities/templates/mobs/spider.hpp>
#include <entities/templates/decor/wave.hpp>
#include <entities/templates/decor/grass.hpp>
#include <entities/templates/decor/symbol.hpp>

#define SYMBOL_NUM 2
#define SPIDER_NUM 3
#define WAVE_SUM 2
#define WILSON_NUM 1

std::chrono::milliseconds totalTime(0);

float randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

Vector3<GLfloat> generateRandomPoint() {
    float MIN_X = -30.0, MAX_X = 30.0;
    float MIN_Y = -3.0, MAX_Y = 3.0;
    float MIN_Z = -64.0, MAX_Z = 2.0;
    float x = randomFloat(MIN_X, MAX_X);
    float y = randomFloat(MIN_Y, MAX_Y);
    float z = randomFloat(MIN_Z, MAX_Z);
    return Vector3<GLfloat>(x, y, z);
}


Scene *createScene()
{
    Ghost *character = nullptr;
    // Wilson *wilson[WILSON_NUM] = {nullptr};
    // Spider *spider[SPIDER_NUM] = {nullptr};
    // Wave *wave[WAVE_SUM] = {nullptr};
    // Symbol *symbol[SYMBOL_NUM] = {nullptr};
    // Grass *grass = nullptr;
    auto *scene = new Scene();

    Ghost::Initialize();
    // Wilson::Initialize();
    // Spider::Initialize();
    // Wave::Initialize();
    // Grass::Initialize();
    // Symbol::Initialize();

    // grass = new Grass();
    // grass->GetTransform()->Move(4, -4, -10);
    // grass->GetTransform()->SetRotate(90, 0, 0);
    // grass->GetTransform()->SetScale(10, 10, 0);
    // grass->updateAnimation("stand");
    // scene->pushActor(grass);


    Sprite symbols(std::string("Symbol"), "shaders/symbol_fs.glsl", "shaders/sprite_vs.glsl", "a", Vector3<GLfloat>(0.0, 0.0, 0.0));
    Sprite arrow(std::string("Arrow"), "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", "img/arrow.png");
    Sprite line(std::string("Arrow"), "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", "img/line.png");


    character = new Ghost();
    character->updateAnimation("stand");
    character->Move(1, Vector3<GLfloat>(0, 0, 1));
    scene->pushActor(character);
    #if MY_ACTOR_TEST
    character->PushRender(GameManager::render);
    #endif

    // for (int i = 0; i < WILSON_NUM; i++) {
    //     wilson[i] = new Wilson();
    //     wilson[i]->Teleport(generateRandomPoint());
    //     wilson[i]->updateAnimation("stand");
    //     scene->pushActor(wilson[i]);
    // }

    // for (int i = 0; i < SPIDER_NUM; i++) {
    //     spider[i] = new Spider();
    //     spider[i]->Teleport(generateRandomPoint());
    //     spider[i]->updateAnimation("stand");
    //     scene->pushActor(spider[i]);
    // }

    // for (int i = 0; i < WAVE_SUM; i++) {
    //     wave[i] = new Wave();
    //     wave[i]->updateAnimation("stand");
    //     objectTransform *transform = wave[i]->GetTransform();
    //     transform->Move(generateRandomPoint());
    //     scene->pushActor(wave[i]);
    // }
    
    // for (int i = 0; i < SYMBOL_NUM; i++) {
    //     symbol[i] = new Symbol();
    //     symbol[i]->updateAnimation("stand");
    //     objectTransform *transform = symbol[i]->GetTransform();
    //     transform->Move(generateRandomPoint());
    //     char s[2] = {'0' + i, '\0'};
    //     symbol[i]->GetSprites()->at("symbol").SetTexture(s);

    //     scene->pushActor(symbol[i]);
    // }

    // for (const auto &it : *symbol[0]->GetSprites()) {
    // std::cout << it.first << std::endl;;
    // }

    Sprite mySprite(std::string("Grass"), "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", "img/cell.jpg");
    GameManager::PushPlayer(character);
    GameManager::render->PushGeometry(mySprite.GetGeometry());
    GameManager::render->pipeline.camera->OwnerTransformPtr = character->GetTransform();

    return scene;
}
