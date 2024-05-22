#include <game/gameManager.hpp>
#include <entities/templates/playable/unit.hpp>
#include <entities/templates/mobs/bullet.hpp>


std::list<Bullet*> bullets;
std::chrono::milliseconds totalTime(0);

bool RenderSceneCB(Render *render, Scene *scene)
{
    static time_t prev = time(0);
    static int frame = 0;
    static const int targetFPS = 120;
    static const int targetMessage = 5;
    static const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    static const std::chrono::milliseconds frameDurationMessage(1000 / targetMessage);
    static std::chrono::steady_clock::time_point framePrevious = std::chrono::steady_clock::now();
    static std::chrono::steady_clock::time_point framePreviousMessage = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point frameEnd = now + frameDuration;
    std::this_thread::sleep_until(frameEnd);

    scene->Time.Update();
    Actor::PushTime(scene->Time.GetCurrentTime());

    GameManager::client.mutex_lock();
    scene->player->MoveForward();
    GameManager::client.mutex_unlock();

    scene->player->UpdateCameraPos();
    scene->player->args.deg = GameManager::deg;
    scene->player->Update();

    for (std::vector<Component>::iterator it = scene->getIterator(); it != scene->component.end(); it++)
        GameManager::render->drawObject(&it->transform, it->sprite);


    // for (auto it = bullets.begin(); it != bullets.end();) {
    //     if ((*it)->GetTransform()->WorldPos.Distance(scene->unit->GetTransform()->WorldPos) < (1.0 + 0.5)) {
    //         if ((*it)->id != scene->unit->id) {
    //             scene->unit->DealingDamage((*it)->params.Damage);
    //             it = bullets.erase(it);
    //             continue;
    //         }
    //     } 
    //     ++it;
    // }


    for (const auto& it : scene->players) {
        std::vector<Component*> ActorComponents = scene->player->getActorComponents();
        for (auto ac : ActorComponents)  GameManager::render->drawObject(&ac->transform, ac->sprite);
    }

    if (scene->player->isFire) {
        Bullet* bullet = scene->player->Fire();
        Vector3<float> direction = scene->player->args.deg;
        GameManager::client.fire(scene->player->GetTransform()->GetWorldPos(), scene->player->args.deg, scene->player->id);
        scene->player->isFire = !scene->player->isFire;
        if (bullet != nullptr) {
            bullets.push_back(bullet);
        }
    }


    // scene->unit->Teleport(Vector3<GLfloat>());
    // scene->unit->args.deg = 0;
    // scene->unit->Update();
    // ActorComponents = scene->unit->getActorComponents();
    // for (auto it : ActorComponents) GameManager::render->drawObject(&it->transform, it->sprite);


    // for (auto blt = bullets.begin(); blt != bullets.end();) {
    //     if ((scene->Time.GetCurrentTime() - (*blt)->GetBirthTime()) > Bullet::lifetime) {
    //         blt = bullets.erase(blt);
    //         if (blt == bullets.end()) break;
    //     } else {
    //         (*blt)->Update();
    //         (*blt)->MoveForward();
    //         ActorComponents = (*blt)->getActorComponents();
    //         for (auto it : ActorComponents) GameManager::render->drawObject(&it->transform, it->sprite);
    //         blt++;
    //     }
    // }


    if (now - framePreviousMessage >= frameDurationMessage) {
        GameManager::client.move(scene->player->GetTransform()->GetWorldPos(), scene->player->args.deg, scene->player->id);
        framePreviousMessage = now;
    }

    return GameManager::IsEnd;
}

