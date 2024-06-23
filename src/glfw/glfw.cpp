#include <game/gameManager.hpp>
// #include <game/gameTime.hpp>
#include <entities/templates/playable/Wilson.hpp>
#include <entities/templates/mobs/spider.hpp>
#include <entities/templates/decor/wave.hpp>

#define SPIDER_NUM 500
#define WAVE_SUM 300

Wilson *character = nullptr;
Spider *spider[SPIDER_NUM] = {nullptr};
Wave *wave[WAVE_SUM] = {nullptr};

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

bool isVisible(Camera *camera, Actor *object)
{
    Vector3<GLfloat> cameraDirection = camera->Params.Target.Normalize();
    Vector3<GLfloat> cameraToObject = (object->GetTransform()->GetWorldPos() - camera->Params.WorldPos).Normalize();

    return cameraDirection.VDot(cameraToObject) > 0;
}

bool RenderSceneCB(Render *render, Scene *scene)
{
    static const int targetFPS = 60;
    static const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    static std::chrono::steady_clock::time_point frameStart;
    std::chrono::steady_clock::time_point frameEnd = std::chrono::steady_clock::now();

    // std::this_thread::sleep_until(frameStart + frameDuration);
    frameStart = std::chrono::steady_clock::now();
    GameManager::Time.Update();

    static time_t prev = time(0);
    static int frame = 0;

    Actor::PushTime(GameManager::Time.GetCurrentTime());

    for (std::vector<Component>::iterator it = scene->getIterator(); it != scene->component.end(); it++)
        GameManager::render->drawObject(&it->transform, it->sprite);

    Camera *camera = character->GetCamera();

    character->MoveForward();
    character->UpdateCameraPos();
    character->updateAnimation(character->GetAnimation(GameManager::Time.GetCurrentTime()));
    std::vector<Component*> ActorComponents = character->getActorComponents();

    for (auto it : ActorComponents) 
        GameManager::render->drawObject(&it->transform, it->sprite);

    for (int i = 0; i < SPIDER_NUM; i++) {
        spider[i]->MoveTowards(character, 0.006);
        if (isVisible(camera, spider[i])) {
            ActorComponents = spider[i]->getActorComponents();
            for (auto it : ActorComponents) 
                GameManager::render->drawObject(&it->transform, it->sprite);
        }
    }


    for (int i = 0; i < WAVE_SUM; i++) {
        if (isVisible(camera, wave[i])) {
            ActorComponents = wave[i]->getActorComponents();
            for (auto it : ActorComponents) 
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

    // std::cout << character->GetTransform() << std::endl;

    return GameManager::IsEnd;
}

Scene *createScene()
{
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
    #if MY_ACTOR_TEST
    character->PushRender(GameManager::render);
    #endif

    for (int i = 0; i < SPIDER_NUM; i++) {
        spider[i] = new Spider();
        spider[i]->Teleport(generateRandomPoint());
        spider[i]->updateAnimation("stand");
    }


    for (int i = 0; i < WAVE_SUM; i++) {
        wave[i] = new Wave();
        wave[i]->updateAnimation("stand");
        objectTransform *transform = wave[i]->GetTransform();
        transform->Move(generateRandomPoint());
    }




    GameManager::PushPlayer(character);
    GameManager::PushCamera(character->GetCamera());
    GameManager::render->SetCamera(character->GetCamera());
    GameManager::render->PushGeometry(mySprite->GetGeometry());

    return scene;
}
