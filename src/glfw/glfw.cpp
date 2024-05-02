#include <game/gameManager.hpp>
#include <game/gameTime.hpp>
#include <entities/templates/playable/Wilson.hpp>
#include <entities/templates/playable/unit.hpp>
#include <entities/templates/mobs/spider.hpp>
#include <entities/templates/decor/wave.hpp>

#include <chrono>
#include <ctime>
#include <random>
#include <thread>


#define SPIDER_NUM 3
#define WAVE_SUM 5

extern Unit *Player;
// extern Spider *spider[];
// extern Wave *wave[];
extern Unit *unit;

std::chrono::milliseconds totalTime(0);

GameTime Time;

bool isVisible(Camera *camera, Actor *object)
{
    Vector3<GLfloat> cameraDirection = camera->Params.Target.Normalize();
    Vector3<GLfloat> cameraToObject = (object->GetTransform()->GetWorldPos() - camera->Params.WorldPos).Normalize();

    return cameraDirection.VDot(cameraToObject) > 0;
}

bool RenderSceneCB(Render *render, Scene *scene)
{
    static time_t prev = time(0);
    static int frame = 0;
    static const int targetFPS = 60;
    static const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    static std::chrono::steady_clock::time_point frameStart;
    std::chrono::steady_clock::time_point frameEnd = std::chrono::steady_clock::now();

    frameStart = std::chrono::steady_clock::now();
    Time.Update();
    Actor::PushTime(Time.GetCurrentTime());

    // std::cout << GameManager::xpos << std::endl;

    for (std::vector<Component>::iterator it = scene->getIterator(); it != scene->component.end(); it++)
        GameManager::render->drawObject(&it->transform, it->sprite);

    Camera *camera = Player->GetCamera();

    Player->MoveForward();
    Player->UpdateCameraPos();
    Player->updateAnimation(Player->GetAnimation(Time.GetCurrentTime()));
    Player->Update(GameManager::deg);
    // std::cout << "camera->Params.WorldPos: " << Player->GetCamera()->Params.WorldPos << std::endl;
    std::vector<Component*> ActorComponents = Player->getActorComponents();

    for (auto it : ActorComponents) 
        GameManager::render->drawObject(&it->transform, it->sprite);

    unit->MoveTowards(Player, 0.01);
    unit->Update(37);
    if (isVisible(camera, unit)) {
        ActorComponents = unit->getActorComponents();
        for (auto it : ActorComponents) 
            GameManager::render->drawObject(&it->transform, it->sprite);
    }


    // for (int i = 0; i < WAVE_SUM; i++) {
    //     if (isVisible(camera, wave[i])) {
    //         ActorComponents = wave[i]->getActorComponents();
    //         for (auto it : ActorComponents) 
    //             GameManager::render->drawObject(&it->transform, it->sprite);
    //     }
    // }


    
    frame++;
    if ((time(0) - prev) > 3) {
        std::cout << "FPS: " << frame / 3 << std::endl;
        prev = time(0);
        frame = 0;
    }

    return GameManager::IsEnd;
}

