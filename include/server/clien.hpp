#pragma once
#include "../lib-project/lib.hpp"
#include "player.hpp"
#include "unit/unitInfo.hpp"

class Client
{
public:
    Client() {
        sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sockfd < 0) {
            perror("Ошибка при создании сокета");
            exit(EXIT_FAILURE);
        }

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
        server_addr.sin_port = htons(PORT);

        if (connect(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Ошибка при установлении соединения");
            exit(EXIT_FAILURE);
        }
    }

    void connect_game() {
        buffer[0] = CLIENT_CONNECT;
        send(sockfd, (const message_type *)&buffer[0], MAX_SIZE, 0);
        // sendto(sockfd, (const message_type *)&buffer[0], MAX_SIZE, 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
        int temp;
        n = recv(sockfd, (int*)&temp, 1, 0);
        mutex_lock();
        id = temp;
        printf("Получено от сервера: %d\n", id);
        mutex_unlock();
    }

    void disconnect_game() {
        buffer[0] = CLIENT_DISCONNECT;
        send(sockfd, (const message_type *)&buffer[0], MAX_SIZE, 0);
    }

    void fire(const Vector3<float>& position, const float direction, const int id) {
        struct FIRE_INFO fire_info = {
            FIRE,
            position,
            direction
        };
        send(sockfd, (const struct FIRE_INFO *)&fire_info, sizeof(struct FIRE_INFO), 0);
    }

    void move(const Vector3<float>& position, const float direction, const int id) {
        struct MOVE_INFO move_info = {
            MOVE,
            position,
            direction
        };
        send(sockfd, (const struct FIRE_INFO *)&move_info, sizeof(struct MOVE_INFO), 0);
    }

    int GetID() { return id; }

    void SetScene(Scene* scene_) { scene = scene_; }

    void mutex_lock()   { pthread_mutex_lock(&player_mutex); }
    void mutex_unlock() { pthread_mutex_unlock(&player_mutex); }


    void move(struct ID_MOVE_INFO* id_move_info) {
        std::cout << "Move!" << std::endl;
        if (id_move_info->id == scene->player->id) {
            mutex_lock();
            scene->player->Teleport(id_move_info->position);
            scene->player->args.deg = id_move_info->direction;
            mutex_unlock();
        } else {
            if (id_move_info->id < 0 || id_move_info->id >= MAX_PLAYERS) return;
            scene->players[id_move_info->id].Teleport(id_move_info->position);
            scene->players[id_move_info->id].args.deg = id_move_info->direction;
        }
    }

    void fire(struct ID_FIRE_INFO* id_fire_info) {
        std::cout << "Fire! " << id_fire_info->id << std::endl;
        move(reinterpret_cast<ID_MOVE_INFO*>(id_fire_info));
        Bullet* bullet = scene->players[id_fire_info->id].Fire();
        if (bullet != nullptr) {
            scene->bullets.push_back(bullet);
        }
    }

    void set_hp(struct ID_SET_HP_INFO* id_set_hp__info) {
        scene->players[id_set_hp__info->id].params.HP = id_set_hp__info->HP;
    }

    void callback() {
        while (true) {
            std::cout << "get" << std::endl;
            n = recv(sockfd, (message_type *)buffer, MAX_SIZE, 0);

            switch (buffer[0])
            {
            case MOVE:
                move(reinterpret_cast<ID_MOVE_INFO*>(buffer));
                break;

            case FIRE:
                fire(reinterpret_cast<ID_FIRE_INFO*>(buffer));
                break;

            case SET_HP:
                set_hp(reinterpret_cast<ID_SET_HP_INFO*>(buffer));
                break;
            
            default:
                break;
            }
        }
    }


// private:
    struct sockaddr_in server_addr;
    int sockfd, n;
    int id = 777;
    message_type buffer[MAX_SIZE];

    Scene* scene = nullptr;
    pthread_mutex_t player_mutex = PTHREAD_MUTEX_INITIALIZER;
};
