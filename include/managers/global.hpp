#pragma once
#include "../entities/actor.hpp"
#include <object/mesh/loader/assimp_mesh_loader.hpp>
#include <object/mesh/loader/tinygltf_mesh_loader.hpp>

class GlobalState  
{
public:
    static void TerminateApp();
    static void SetPlayer(Actor* newPlayer);
    static Actor* GetPlayer();

    inline static IMeshLoader* MeshLoader = new AssimpMeshLoader;
    inline static IMeshLoader* MeshLoaderTiny = new TinygltfMeshLoader;
    inline static Actor* player = nullptr;
    inline static bool fIsAppRunning = true;
    inline static bool fScreenshot = false;
    inline static int ScreenshotCounter = 0;
    inline static int FrameCounter = 0;
    inline static int FPS = 0;
};
