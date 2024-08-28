#pragma once

#include <ft2build.h>
#include "gameTime.hpp"
#include "../render/glfw.hpp"
#include "../render/render.hpp"
#include "../object/sprite.hpp"
#include "../entities/character.hpp"
#include "../entities/templates/playable/player.hpp"
#include "../threads/threads.hpp"

#include FT_FREETYPE_H
#include FT_ERRORS_H

enum {
    FORWARD,
    BACK,
    RIGHT,
    LEFT
};

struct Character {
    GLuint TextureID;  // ID текстуры
    glm::ivec2 Size;   // Размер глифа
    glm::ivec2 Bearing;  // Смещение от базовой линии
    GLuint Advance;  // Горизонтальное смещение для следующего символа
};


class GameManager  
{
public:
    GameManager ();
    ~GameManager ();

    static void Intit_characters();
    static void PushCamera(Camera *_camera);
    static void PushPlayer(Pawn *_player);
    static Camera *createCamera();
    static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void InitializeObjects();
    static void InitializeGLFW(int _width, int _height);

    struct CallbackData {
        Camera* camera = nullptr;
        Pawn* player = nullptr;
    };


    inline static GameTime Time;

    inline static FT_Library ft = nullptr;
    inline static std::map<GLchar, Character> *Characters = nullptr;

    inline static SceneThread* threads = nullptr;

    inline static Render* render = nullptr;

    inline static CallbackData callbackData = {nullptr, nullptr};
    inline static GLFWwindow* window = nullptr;
    inline static int width = 640;
    inline static int height = 480;
    inline static bool IsEnd = false;
};
