#pragma once
#include "../lib-project/lib.hpp"

float RandF();

class Player
{
public:
    Player(const Player& player_) : client_addr(player_.client_addr), sockfd(player_.sockfd) {
        params.id = player_.params.id;
        params.position = Vector3<float>(RandF(), RandF(), 0.0);
    };
    Player(const struct sockaddr_in& client_addr_, const int id_) : client_addr(client_addr_) {
        sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sockfd < 0) {
            perror("Ошибка при создании сокета");
            exit(EXIT_FAILURE);
        }

        if (connect(sockfd, (const struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
            perror("Ошибка при установлении соединения");
            exit(EXIT_FAILURE);
        }

        params.id = id_;
        params.position = Vector3<float>(RandF(), RandF(), 0.0);
        // params.position = DegToPnt(static_cast<float>(360 / MAX_PLAYERS * params.id)) * 5;
    };


    bool operator==(const Player& other) const {
        std::cout << client_addr.sin_addr.s_addr << " " << client_addr.sin_port << std::endl;
        std::cout << other.client_addr.sin_addr.s_addr << " " << other.client_addr.sin_port << std::endl << std::endl;

        return client_addr.sin_addr.s_addr == other.client_addr.sin_addr.s_addr && client_addr.sin_port == other.client_addr.sin_port;
    }

    bool operator==(const struct sockaddr_in& other) const {
        std::cout << client_addr.sin_addr.s_addr << " " << client_addr.sin_port << std::endl;
        std::cout << other.sin_addr.s_addr << " " << other.sin_port << std::endl << std::endl;

        return client_addr.sin_addr.s_addr == other.sin_addr.s_addr && client_addr.sin_port == other.sin_port;
    }


    struct sockaddr_in client_addr;
    int sockfd;

    Vector3<float> wrong_offset;
    struct player_params {
        Vector3<float> position;
        float direction;
        int HP = MAX_HP;
        int id;
    } params;
};
