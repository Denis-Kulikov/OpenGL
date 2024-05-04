#pragma once
#include "../lib-project/lib.hpp"

class Player
{
public:
    Player(const struct sockaddr_in& client_addr_, const int id_)
        : client_addr(client_addr_), id(id_) {};


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


// private:
    struct sockaddr_in client_addr;
    Vector3<float> position;
    int id;
};
