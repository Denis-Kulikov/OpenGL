#pragma once

#include "../render/glfw.hpp"
#include "../render/render.hpp"
#include "../object/sprite.hpp"
#include "../entities/character.hpp"
#include <entities/templates/playable/unit.hpp>


class GameManager  
{
public:
    GameManager () {};

    std::string name;
    objectTransform trans;

    static void PushCamera(Camera *_camera);
    static void PushPlayer(Unit *_player);

    static void DrawSphere();

    static void InitializeObjects();
    static Camera *createCamera();
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods);
    static Render *InitializeGLFW(GLFWwindow* &window, int _width, int _height);

    inline static float deg;
    static bool IsEnd;
    static int width;
    static int height;
    static Render *render;
    
    struct CallbackData {
        Camera* camera;
        Unit* player;
    };
// protected:
    static CallbackData callbackData;
};
