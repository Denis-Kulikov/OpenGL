#pragma once
#include "../entities/actor.hpp"

class GlobalState  
{
public:
    static void TerminateApp();
    static void SetPlayer(Actor* newPlayer);
    static Actor* GetPlayer();

    inline static Actor* player = nullptr;
    inline static bool fIsAppRunning = true;
    inline static bool fScreenshot = false;
    inline static int ScreenshotCounter = 0;
    inline static int FrameCounter = 0;
    inline static int FPS = 0;
};
