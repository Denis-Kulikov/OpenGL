#pragma once
#include "../lib-project/lib.hpp"
#include "player.hpp"
#include "unit/unitInfo.hpp"
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
            auto player_it = players.find(player_info);;

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
                        sendto(sockfd, (int*)&player_id, 1, 0, (const struct sockaddr *)&client_addr, len);
                    } else {
                        player_id = (*player_it).second.params.id;
                        std::cout << "Игрок уже существует" << std::endl;
                        sendto(sockfd, (int*)&player_id, 1, 0, (const struct sockaddr *)&client_addr, len);
                    }
                }
            } else {
                if (player_it == players.end()) break;
                player = &player_it->second;
                // std::cout << "ID: " << player->params.id;


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
                    UnitInfo::UnitFire(fire_info->position, fire_info->direction, player->params.id);

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

                    // if ((offset * (1000 / TPS)).Length() > PLAYER_SPEED) {
                    //     if (((offset + player->wrong_offset) * (1000 / TPS)).Length() > PLAYER_SPEED) {
                    //         player->params.position = move_info->position + player->wrong_offset;
                    //     } else {
                    //         offset = offset.Normalize() * PLAYER_SPEED; 
                    //         player->params.position += offset;


                    //         send(player->sockfd, (const struct ID_MOVE_INFO *)&id_move_info, sizeof(struct ID_MOVE_INFO), 0);
                    //         // sendto(sockfd, (int*)&player_id, 1, 0, (const struct sockaddr *)&client_addr, len);
                    //     }

                    // } else {
                    //     player->params.position = move_info->position;
                    // }
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


                    // sendto(client_addr, (const struct ID_MOVE_INFO *)&id_move_info, sizeof(struct ID_MOVE_INFO), 0);
                    // send(player->sockfd, (const struct ID_MOVE_INFO *)&id_move_info, sizeof(struct ID_MOVE_INFO), 0);

                    // sendto(sockfd, (const struct ID_MOVE_INFO *)&id_move_info, sizeof(struct ID_MOVE_INFO), 0, (const struct sockaddr *)&(player->client_addr), sizeof(player->client_addr));
                    // sendto(sockfd, (const struct ID_MOVE_INFO *)&id_move_info, 1, 0, (const struct sockaddr *)&client_addr, len);
                    break;
                }

                default:
                    break;
                }
            }
        }
    }


private:
    std::map<std::pair<int, int>, Player> players;
    struct sockaddr_in server_addr;
    int sockfd, n;
    bool id[MAX_PLAYERS];
    message_type buffer[MAX_SIZE];
};
