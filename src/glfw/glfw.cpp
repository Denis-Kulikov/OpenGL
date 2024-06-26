#include <game/gameManager.hpp>
// #include <game/gameTime.hpp>
#include <entities/templates/playable/Wilson.hpp>
#include <entities/templates/mobs/spider.hpp>
#include <entities/templates/decor/wave.hpp>

#define SPIDER_NUM 3000
#define WAVE_SUM 2000

std::chrono::milliseconds totalTime(0);

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
    Wilson *character = nullptr;
    Spider *spider[SPIDER_NUM] = {nullptr};
    Wave *wave[WAVE_SUM] = {nullptr};


    auto *scene = new Scene(std::string("Main scene"));

    Sprite *mySprite = new Sprite(std::string("Grass"), "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", "img/grass.png");
    objectTransform transformGrass;
    transformGrass.Move(0, -5, -2);
    transformGrass.SetRotate(90, 0, 0);
    transformGrass.SetScale(10, 10, 0);
    Component component(transformGrass, mySprite);
    scene->pushObject(component);

    Wilson::Initialize();
    Spider::Initialize();
    Wave::Initialize();

    character = new Wilson();
    character->createCamera(GameManager::width, GameManager::height);
    character->updateAnimation("stand");
    scene->pushActor(character);
    #if MY_ACTOR_TEST
    character->PushRender(GameManager::render);
    #endif

    for (int i = 0; i < SPIDER_NUM; i++) {
        spider[i] = new Spider();
        spider[i]->Teleport(generateRandomPoint());
        spider[i]->updateAnimation("stand");
        scene->pushActor(spider[i]);
    }


    for (int i = 0; i < WAVE_SUM; i++) {
        wave[i] = new Wave();
        wave[i]->updateAnimation("stand");
        objectTransform *transform = wave[i]->GetTransform();
        transform->Move(generateRandomPoint());
        scene->pushActor(wave[i]);
    }


    GameManager::PushPlayer(character);
    GameManager::PushCamera(character->GetCamera());
    GameManager::render->SetCamera(character->GetCamera());
    GameManager::render->PushGeometry(mySprite->GetGeometry());

    return scene;
}
