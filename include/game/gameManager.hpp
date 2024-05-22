#pragma once

#include "../render/glfw.hpp"
#include "../render/render.hpp"
#include "../object/sprite.hpp"
#include "../entities/character.hpp"
#include "../server/clien.hpp"
#include <entities/templates/playable/unit.hpp>


class GameManager  
{
public:
    GameManager () {};

    std::string name;
    objectTransform trans;

    static void PushScene(Scene *_scene);
    static void PushCamera(Camera *_camera);
    static void PushPlayer(Unit *_player);

    static void DrawSphere();

    static void InitializeObjects();
    static Camera *createCamera();
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods);
    static Render *InitializeGLFW(int _width, int _height);

    static inline float deg = 0;
    static inline bool IsEnd = false;
    static inline int width = 0;
    static inline int height = 0;
    static inline Render *render = nullptr;
    static inline Scene *scene = nullptr;
    static inline GLFWwindow* window = nullptr;
    static inline Client client;
    
    struct CallbackData {
        Camera* camera;
        Unit* player;
    };
// protected:
    static inline CallbackData callbackData = { nullptr, nullptr };
};
