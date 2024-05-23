#include <game/gameTime.hpp>
#include <server/server.hpp>
#include <thread>

int main(int argc, char** argv)
{
    Server server;

    std::thread client_callback([&]() {
        while(true) server.tick();
    });
    client_callback.detach();
    server.callback();

    return 0;
}
