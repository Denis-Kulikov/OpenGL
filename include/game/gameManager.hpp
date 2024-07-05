#pragma once

#include "gameTime.hpp"
#include "../render/glfw.hpp"
#include "../render/render.hpp"
#include "../object/sprite.hpp"
#include "../entities/character.hpp"
#include "../entities/templates/playable/player.hpp"
#include "../threads/threads.hpp"

enum {
    FORWARD,
    BACK,
    RIGHT,
    LEFT
};

class GameManager  
{
public:
    GameManager ();
    ~GameManager ();

    static void PushCamera(Camera *_camera);
    static void PushPlayer(Character *_player);
    static Camera *createCamera();
    static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void InitializeObjects();
    static void InitializeGLFW(int _width, int _height);

    struct CallbackData {
        Camera* camera = nullptr;
        Character* player = nullptr;
    };

    inline static GameTime Time;
    inline static SceneThread* threads = nullptr;
    inline static Render* render = nullptr;
    inline static CallbackData callbackData = {nullptr, nullptr};
    inline static GLFWwindow* window = nullptr;
    inline static int width = 640;
    inline static int height = 480;
    inline static bool IsEnd = false;
};
