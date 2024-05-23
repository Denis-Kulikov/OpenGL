#include <game/gameManager.hpp>
#include <entities/templates/playable/unit.hpp>
#include <entities/templates/mobs/bullet.hpp>


std::chrono::milliseconds totalTime(0);

bool RenderSceneCB(Render *render, Scene *scene)
{
    static time_t prev = time(0);
    static int frame = 0;
    static const int targetFPS = 120;
    static const int targetMessage = 40;
    static const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    static const std::chrono::milliseconds frameDurationMessage(1000 / targetMessage);
    static std::chrono::steady_clock::time_point framePrevious = std::chrono::steady_clock::now();
    static std::chrono::steady_clock::time_point framePreviousMessage = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point frameEnd = now + frameDuration;
    std::this_thread::sleep_until(frameEnd);

    scene->Time.Update();
    Actor::PushTime(scene->Time.GetCurrentTime());
    for (std::vector<Component>::iterator it = scene->getIterator(); it != scene->component.end(); it++)
        GameManager::render->drawObject(&it->transform, it->sprite);


    GameManager::client.mutex_lock();
    scene->player->MoveForward();
    GameManager::client.mutex_unlock();

    scene->player->UpdateCameraPos();
    scene->player->args.deg = GameManager::deg;

    // Коллизий пуль
    for (auto blt = scene->bullets.begin(); blt != scene->bullets.end();) {
        if ((scene->Time.GetCurrentTime() - (*blt)->GetBirthTime()) > Bullet::lifetime) {
            blt = scene->bullets.erase(blt);
            if (blt == scene->bullets.end()) break;
        }
        for (auto& plr : GameManager::scene->players) {
            if ((plr.first != (*blt)->id) && (plr.second.params.HP > 0)) {
                if ((*blt)->GetTransform()->WorldPos.Distance(plr.second.GetTransform()->WorldPos) < (1.0 + 0.5)) {
                    blt = scene->bullets.erase(blt);
                    break;
                }
            }
        }
        if (blt == scene->bullets.end()) break;
        ++blt;
    }

    // отрисовка игроков
    for (auto& it : scene->players) {
        it.second.Update();
        std::vector<Component*> ActorComponents = it.second.getActorComponents();
        for (auto ac : ActorComponents)  GameManager::render->drawObject(&ac->transform, ac->sprite);
    }

    // выстрелы
    if (scene->player->isFire) {
        Bullet* bullet = scene->player->Fire();
        GameManager::client.fire(scene->player->GetTransform()->GetWorldPos(), scene->player->args.deg, scene->player->id);
        scene->player->isFire = false;
        if (bullet != nullptr) {
            scene->bullets.push_back(bullet);
        }
    }

    // отрисовка пуль
    for (auto blt = scene->bullets.begin(); blt != scene->bullets.end();) {
        std::vector<Component*> ActorComponents;
        (*blt)->Update();
        (*blt)->MoveForward();
        ActorComponents = (*blt)->getActorComponents();
        for (auto it : ActorComponents) GameManager::render->drawObject(&it->transform, it->sprite);
        blt++;
    }


    if (now - framePreviousMessage >= frameDurationMessage) {
        GameManager::client.move(scene->player->GetTransform()->GetWorldPos(), scene->player->args.deg, scene->player->id);
        framePreviousMessage = now;
    }

    return GameManager::IsEnd;
}

