#pragma once

#include "gameTime.hpp"
//  #include "bulletManager.hpp"
#include "../render/render.hpp"
#include "../object/sprite.hpp"
#include "../entities/character.hpp"
#include "../entities/templates/playable/player.hpp"

struct GLFWwindow;

class GameManager  
{
public:
    GameManager ();
    ~GameManager ();

    static void PushCamera(const Camera &_camera);
    static void PushPlayer(Character *_player);
    static void MouseCB(GLFWwindow* window, double xpos, double ypos);
    static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void UpdateCamera();

    static void InitializeObjects();
    static void InitializeGLFW(int _width, int _height);

    struct CallbackData {
        Character* player = nullptr;
    };

    struct Buttons {
        int yaw = 0;
        int pitch = 0;

        float lastX = 400, lastY = 300;

        bool cursor_disable = true;
        bool firstMouse = true;
    };

    inline static GLuint framebuffer, texture, depthbuffer;

    inline static Buttons buttons = {0, 0, 400, 300, true, true};
    inline static GameTime Time;
    // inline static SceneThread* threads = nullptr;
    inline static Render render;
    inline static CallbackData callbackData = {nullptr};
    // inline static BulletManager bullet;
    inline static int width = 640;
    inline static int height = 480;
    inline static bool IsEnd = false;
};
