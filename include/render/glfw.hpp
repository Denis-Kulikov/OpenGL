#pragma once
#include "../lib-project/lib.hpp"
#include "../object/scene.hpp"
#include "render.hpp"

#define PI = 3.14159265359f;

void GameCallback();
bool RenderSceneCB(Render *render, Scene *scene);
Scene *createScene(int id);
Render *InitializeGLFW(GLFWwindow* &window, int width, int height);
