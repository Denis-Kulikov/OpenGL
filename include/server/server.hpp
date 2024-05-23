#pragma once
#include "../lib-project/lib.hpp"
#include "player.hpp"
// #include "unit/unitInfo.hpp"
#include "../entities/templates/playable/unit.hpp"
#include "../entities/templates/mobs/bullet.hpp"
#include <map>
#include <utility>
#include <set>

#define TPS 30

int generateRandomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);

    return dis(gen);
}

Unit gun;

class Server
{
public:
    Server() {
        sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sockfd < 0) {
            perror("Ошибка при создании сокета");
            exit(EXIT_FAILURE);
        }

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(PORT);

        if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Ошибка при привязке сокета");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < MAX_PLAYERS; i++)  id[i] = true;
    }

    int CreatePlayer(const struct sockaddr_in& client_addr_) {
        std::pair<int, int> player_info(client_addr_.sin_addr.s_addr, client_addr_.sin_port);
        int free_id;
        for (free_id = 0; free_id < MAX_PLAYERS; free_id++) {
            if (id[free_id]) break;
        }

        if (free_id == MAX_PLAYERS) return free_id;

        Player player(client_addr_, free_id);
        players.insert(std::make_pair(player_info, player));
        id[free_id] = false;

        for (int i = 0; i < MAX_PLAYERS; i++) {
            std::cout << id[i] << " ";
        }
        std::cout << std::endl; 

        return free_id;
    }

    void callback() {
        struct sockaddr_in client_addr;
        unsigned int len = sizeof(client_addr);
        while (true) {
            n = recvfrom(sockfd, (message_type *)buffer, MAX_SIZE, 0, (struct sockaddr *)&client_addr, &len);
            std::pair<int, int> player_info(client_addr.sin_addr.s_addr, client_addr.sin_port);
            Player *player = nullptr;
            auto player_it = players.find(player_info);

            if (buffer[0] == CLIENT_CONNECT) {
                printf("Получено от клиента %s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);
                int player_id;
                std::cout << "Size: " << players.size() << std::endl;

                if (players.size() >= MAX_PLAYERS) {
                    player_id = MAX_PLAYERS;
                    std::cout << "Сервер заполнен" << std::endl;
                    sendto(sockfd, (int*)&player_id, 1, 0, (const struct sockaddr *)&client_addr, len);
                } else {
                    if (player_it == players.end()) {
                        player_id = CreatePlayer(client_addr);
                        std::cout << "Send id " << player_id << std::endl;
                        player_it = players.begin();
                        while (player_it->second.params.id != player_id) ++player_it;
                        struct ID_MOVE_INFO id_move_info = {
                            CLIENT_CONNECT,
                            player_it->second.params.position,
                            player_it->second.params.direction,
                            player_it->second.params.id
                        };
                        sendto(sockfd, (const struct ID_MOVE_INFO *)&id_move_info, sizeof(struct ID_MOVE_INFO), 0, (const struct sockaddr *)&(client_addr), sizeof(client_addr));
                        for (const auto& it : players) {
                            if (it.second.params.id == player_id) continue;
                            sendto(sockfd, (const struct ID_MOVE_INFO *)&id_move_info, sizeof(struct ID_MOVE_INFO), 0, (const struct sockaddr *)&(it.second.client_addr), sizeof(it.second.client_addr));
                        }
                    } else {
                        player_id = (*player_it).second.params.id;
                        std::cout << "Игрок уже существует" << std::endl;
                        sendto(sockfd, (int*)&player_id, 1, 0, (const struct sockaddr *)&client_addr, len);
                    }
                }
            } else {
                if (player_it == players.end()) break;
                player = &player_it->second;

                switch (buffer[0])
                {
                case CLIENT_DISCONNECT:
                {
                    printf("Игрок #%d покинул игру.\n", player->params.id);

                    id[player->params.id] = true;
                    players.erase(player_it);          
                    break;
                }

                case FIRE:
                {
                    struct FIRE_INFO* fire_info = reinterpret_cast<FIRE_INFO*>(buffer);
                    gun.SetID(player->params.id);
                    gun.Teleport(fire_info->position);
                    gun.args.deg = fire_info->direction;
                    gun.SetID(player->params.id);

                    Bullet* bullet = gun.Fire();
                    if (bullet != nullptr) {
                        bullets.push_back(bullet);
                    }
                    // UnitInfo::UnitFire(fire_info->position, fire_info->direction, player->params.id);

                    struct ID_FIRE_INFO id_fire_info = {
                        FIRE,
                        player->params.position,
                        player->params.direction,
                        player->params.id
                    };

                    for (const auto& it : players) {
                        if (it.second.params.id == player->params.id) continue;
                        sendto(sockfd, (const struct ID_FIRE_INFO *)&id_fire_info, sizeof(struct ID_FIRE_INFO), 0, (const struct sockaddr *)&(it.second.client_addr), sizeof(it.second.client_addr));
                    }

                          
                    break;
                }

                case MOVE:
                {
                    struct MOVE_INFO* move_info = reinterpret_cast<MOVE_INFO*>(buffer);
                    Vector3<float> offset = move_info->position;
                    offset -= player->params.position;


                    player->params.position = move_info->position;
                    player->params.direction = move_info->direction;


                    struct ID_MOVE_INFO id_move_info = {
                        MOVE,
                        player->params.position,
                        player->params.direction,
                        player->params.id
                    };
                    for (const auto& it : players) {
                        if (it.second.params.id == player->params.id) continue;
                        sendto(sockfd, (const struct ID_MOVE_INFO *)&id_move_info, sizeof(struct ID_MOVE_INFO), 0, (const struct sockaddr *)&(it.second.client_addr), sizeof(it.second.client_addr));
                    }
                    break;
                }

                default:
                    break;
                }
            }
        }
    }

    void DealingDamage(Player& player_, int damage) {
        player_.params.HP -= damage;
        struct ID_SET_HP_INFO id_set_hp_info = {
            SET_HP,
            player_.params.HP,
            player_.params.id
        };
        for (const auto& it : players) {
            sendto(sockfd, (const struct ID_SET_HP_INFO *)&id_set_hp_info, sizeof(struct ID_SET_HP_INFO), 0, (const struct sockaddr *)&(it.second.client_addr), sizeof(it.second.client_addr));
        }
        if (player_.params.HP <= 0) {
            auto it = players.begin();
            while (it->second.params.id != player_.params.id) ++it;
            struct ID_DISCONNECT id_disconnect = {
                CLIENT_DISCONNECT,
                player_.params.id
            };
            sendto(sockfd, (const struct ID_DISCONNECT *)&id_disconnect, sizeof(struct ID_DISCONNECT), 0, (const struct sockaddr *)&(it->second.client_addr), sizeof(it->second.client_addr));
            players.erase(it);          
        }
    }

    void tick() {
        static const int targetFPS = 33;
        static const std::chrono::milliseconds frameDuration(1000 / targetFPS);
        static std::chrono::steady_clock::time_point framePrevious = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point frameEnd = now + frameDuration;
        std::this_thread::sleep_until(frameEnd);

        Time.Update();
        static float cut = Time.GetCurrentTime();
        static float prev_time = Time.GetCurrentTime();
        Time.Update();
        cut = (Time.GetCurrentTime() - prev_time) * 1e-8;
        prev_time = Time.GetCurrentTime();
        Actor::PushTime(Time.GetCurrentTime());


        // Коллизия пуль
        for (auto blt = bullets.begin(); blt != bullets.end();) {
            (*blt)->MoveForward(cut * (*blt)->GetSpeed());
            if ((Time.GetCurrentTime() - (*blt)->GetBirthTime()) > Bullet::lifetime) {
                blt = bullets.erase(blt);
                if (blt == bullets.end()) break;
            }
            for (auto& plr : players) {
                if ((plr.second.params.id != (*blt)->id) && (plr.second.params.HP > 0)) {
                    if ((*blt)->GetTransform()->WorldPos.Distance(plr.second.params.position) < (1.0 + 0.5)) {
                        std::cout << "DealingDamage" << std::endl;
                        DealingDamage(plr.second, BULLET_DAMAGE);
                        blt = bullets.erase(blt);
                        break;
                    }
                }
            }
            if (blt == bullets.end()) break;
            ++blt;
        }
    }


private:
    std::map<std::pair<int, int>, Player> players;
    struct sockaddr_in server_addr;
    int sockfd, n;
    bool id[MAX_PLAYERS];
    message_type buffer[MAX_SIZE];
    std::list<Bullet*> bullets;
    GameTime Time;
};
