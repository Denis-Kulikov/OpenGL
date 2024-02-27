#pragma once
#include "../lib-project/lib.hpp"
#include "../object/scene.hpp"
#include "render.hpp"

#define PI = 3.14159265359f;

struct CallbackData {
    Camera* camera;
};


bool RenderSceneCB(Render *render, Scene *scene);
Scene *createScene();
Render *InitializeGLFW(GLFWwindow* &window, int width, int height);
