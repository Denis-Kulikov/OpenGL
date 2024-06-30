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

    std::string name;
    objectTransform trans;

    static void PushCamera(Camera *_camera);
    static void PushPlayer(Character *_player);

    static void DrawSphere();

    static void InitializeObjects();
    static Camera *createCamera();
    static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods);
    static Render *InitializeGLFW(int _width, int _height);

    struct CallbackData {
        Camera* camera;
        Character* player;
    };

    inline static bool IsEnd = false;
    inline static int width = 640;
    inline static int height = 480;
    inline static GLFWwindow* window = nullptr;
    inline static Render *render = nullptr;
    inline static GameTime Time;
    inline static SceneThread threads;
    inline static CallbackData callbackData = {nullptr, nullptr};
};
