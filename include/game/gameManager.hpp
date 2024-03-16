#pragma once

#include "../render/glfw.hpp"
#include "../render/render.hpp"
#include "../object/sprite.hpp"
#include "../entities/character.hpp"
#include "../entities/templates/playable/player.hpp"


class GameManager  
{
public:
    GameManager () {};

    std::string name;
    objectTransform trans;

    static void PushCamera(Camera *_camera);
    static void PushPlayer(Player *_player);

    static void InitializeObjects();
    static Camera *createCamera();
    static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods);
    static Render *InitializeGLFW(GLFWwindow* &window, int _width, int _height);

    static bool IsEnd;
    static int width;
    static int height;
    static Render *render;
    
    struct CallbackData {
        Camera* camera;
        Player* player;
    };
protected:
    static CallbackData callbackData;
};
