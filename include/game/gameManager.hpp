#pragma once

#include "../render/glfw.hpp"
#include "../render/render.hpp"
#include "../object/sprite.hpp"

class GameManager  
{
public:
    GameManager () {};

    std::string name;
    objectTransform trans;

    static void PushCamera(Camera *_camera);
    static void PushPlayerTransform(objectTransform *_transform);

    static void InitializeObjects();
    static Camera *createCamera(int width, int height);
    static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods);
    static Render *InitializeGLFW(GLFWwindow* &window, int width, int height);

    static bool IsEnd;
protected:
    static CallbackData callbackData;
};
