#pragma once
#include "../entities/character.hpp"

class GlobalState  
{
public:
    static void TerminateApp();
    static void SetPlayer(Pawn* newPlayer);
    static Pawn* GetPlayer();

    inline static Pawn* player = nullptr;
    inline static bool isAppRunning = true;
};
