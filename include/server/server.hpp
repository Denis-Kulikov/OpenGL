#pragma once
#include "../lib-project/lib.hpp"
#include "player.hpp"


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

        int free_id;
        for (free_id = 0; free_id < MAX_PLAYERS; free_id++) {
            if (id[free_id]) break;
        }

        if (free_id == MAX_PLAYERS) return free_id;

        Player* player = new Player(client_addr_, free_id);
        players.push_back(player);
        id[free_id] = false;

        for (int i = 0; i < MAX_PLAYERS; i++) {
            std::cout << id[i] << " ";
        }
        std::cout << std::endl; 

        return free_id;
    }

    void callback()
    {
        struct sockaddr_in client_addr;
        unsigned int len = sizeof(client_addr);
        while (true) {
            n = recvfrom(sockfd, (char *)buffer, MAX_SIZE, 0, (struct sockaddr *)&client_addr, &len);
            buffer[n] = '\0';

            if (std::string(buffer) == std::string("GetID")) {
                printf("Получено от клиента %s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);
                bool finded;
                std::cout << "Size: " << players.size() << std::endl;
                for (const auto& it : players) {
                    if (finded = (*it == client_addr))
                        break;
                }
                if (!finded) {
                    int player_id = CreatePlayer(client_addr);
                    sprintf(buffer, "%d", player_id);
                    sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&client_addr, len);
                } else {
                    sprintf(buffer, "%d", MAX_PLAYERS);
                    sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&client_addr, len);
                }
            }

        }
    }


private:
    std::vector<Player*> players;
    struct sockaddr_in server_addr;
    int sockfd, n;
    bool id[MAX_PLAYERS];
    char buffer[MAX_SIZE];
};
