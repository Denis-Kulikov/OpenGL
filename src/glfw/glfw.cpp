#include <game/gameManager.hpp>
#include <entities/templates/playable/unit.hpp>
#include <entities/templates/mobs/bullet.hpp>

#include <chrono>
#include <ctime>
#include <random>
#include <thread>


std::list<Bullet*> bullets;
std::chrono::milliseconds totalTime(0);

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
    static const int targetFPS = 120;
    static const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    static std::chrono::steady_clock::time_point frameStart;
    std::chrono::steady_clock::time_point frameEnd = std::chrono::steady_clock::now();
    frameStart = frameEnd;
    frameEnd += frameDuration;
    std::this_thread::sleep_until(frameEnd);

    scene->Time.Update();
    Actor::PushTime(scene->Time.GetCurrentTime());

    for (std::vector<Component>::iterator it = scene->getIterator(); it != scene->component.end(); it++)
        GameManager::render->drawObject(&it->transform, it->sprite);


    for (auto it = bullets.begin(); it != bullets.end();) {
        if ((*it)->GetTransform()->WorldPos.Distance(scene->unit->GetTransform()->WorldPos) < (1.0 + 0.5)) {
            if ((*it)->owner != scene->unit->id) {
                scene->unit->DealingDamage((*it)->params.Damage);
                it = bullets.erase(it);
                continue;
            }
        } 
        ++it;
    }


    scene->player->MoveForward();
    scene->player->UpdateCameraPos();
    scene->player->args.deg = GameManager::deg;
    scene->player->Update();
    std::vector<Component*> ActorComponents = scene->player->getActorComponents();

    if (scene->player->isFire) {
        Bullet* bullet = scene->player->Fire();
        scene->player->isFire = !scene->player->isFire;
        if (bullet != nullptr) {
            bullets.push_back(bullet);
        }
    }

    for (auto it : ActorComponents)  GameManager::render->drawObject(&it->transform, it->sprite);

    // scene->unit->Teleport(Vector3<GLfloat>());
    scene->unit->MoveTowards(scene->player, 0.01);
    scene->unit->args.deg = 0;
    scene->unit->Update();
    ActorComponents = scene->unit->getActorComponents();
    for (auto it : ActorComponents) GameManager::render->drawObject(&it->transform, it->sprite);


    for (auto blt = bullets.begin(); blt != bullets.end();) {
        if ((scene->Time.GetCurrentTime() - (*blt)->GetBirthTime()) > Bullet::lifetime) {
            blt = bullets.erase(blt);
            if (blt == bullets.end()) break;
        } else {
            (*blt)->Update();
            (*blt)->MoveForward();
            ActorComponents = (*blt)->getActorComponents();
            for (auto it : ActorComponents) GameManager::render->drawObject(&it->transform, it->sprite);
            blt++;
        }
    }

    
    // frame++;
    // if ((time(0) - prev) > 3) {
    //     std::cout << "FPS: " << frame / 3 << std::endl;
    //     prev = time(0);
    //     frame = 0;
    // }

    return GameManager::IsEnd;
}

