#pragma once
#include "../lib-project/lib.hpp"
#include "../object/scene.hpp"
#include "render.hpp"

#define PI = 3.14159265359f;

using GLfloat = GLfloat; // for vs code

bool RenderSceneCB(Render *render, Scene *scene);
Scene *createScene();
Render *InitializeGLFW(GLFWwindow* &window, int width, int height);
