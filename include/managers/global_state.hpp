#pragma once
#include "../entities/actor.hpp"

class GlobalState  
{
public:
    static void TerminateApp();
    static void SetPlayer(Actor* newPlayer);
    static Actor* GetPlayer();

    inline static Actor* player = nullptr;
    inline static bool isAppRunning = true;
};
