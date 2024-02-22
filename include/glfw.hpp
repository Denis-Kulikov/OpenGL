#include <fstream>
#include <iostream>
#include <cmath>
#include <cassert>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../include/distance.hpp"
#include "../include/math_3d.hpp"
#include "../include/pipeline.hpp"

using namespace std;

#define PI = 3.14159265359f;


bool RenderSceneCB();
GLuint LoadTexures(const char *texture_path);
void CompileShaders();
void InitializeGLFW(GLFWwindow* &window);
