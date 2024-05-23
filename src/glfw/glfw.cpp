#include <game/gameManager.hpp>
#include <entities/templates/playable/unit.hpp>
#include <entities/templates/mobs/bullet.hpp>

float RandF() {
    float min = -7.5;
    float max = 7.5;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);

    return dis(gen);
}

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
    static std::chrono::steady_clock::time_point previous = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    // std::chrono::steady_clock::time_point cut = now - previous;
    // std::chrono::steady_clock::time_point frameEnd = now + frameDuration;
    // std::this_thread::sleep_until(frameEnd);

    scene->Time.Update();
    static float cut = scene->Time.GetCurrentTime();
    static float prev_time = scene->Time.GetCurrentTime();
    scene->Time.Update();
    cut = (scene->Time.GetCurrentTime() - prev_time) * 1e-8;
    prev_time = scene->Time.GetCurrentTime();
    // std::cout << cut << std::endl;
    Actor::PushTime(scene->Time.GetCurrentTime());
    for (std::vector<Component>::iterator it = scene->getIterator(); it != scene->component.end(); it++)
        GameManager::render->drawObject(&it->transform, it->sprite);


    GameManager::client.mutex_lock();
    scene->player->MoveForward(static_cast<float>(cut) * scene->player->GetSpeed());
    GameManager::client.mutex_unlock();

    scene->player->UpdateCameraPos();
    scene->player->args.deg = GameManager::deg;

    // Коллизия пуль
    for (auto blt = scene->bullets.begin(); blt != scene->bullets.end();) {
        (*blt)->MoveForward(cut * (*blt)->GetSpeed());
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
        ActorComponents = (*blt)->getActorComponents();
        for (auto it : ActorComponents) GameManager::render->drawObject(&it->transform, it->sprite);
        blt++;
    }


    if (now - framePreviousMessage >= frameDurationMessage) {
        GameManager::client.move(scene->player->GetTransform()->GetWorldPos(), scene->player->args.deg, scene->player->id);
        framePreviousMessage = now;
    }

    std::chrono::steady_clock::time_point frameEnd = now + frameDuration;
    std::this_thread::sleep_until(frameEnd);

    return GameManager::IsEnd;
}

