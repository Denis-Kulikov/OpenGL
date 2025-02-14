#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <set>
#include <stack>
#include <utility>
#include <vector>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <mutex>
#include <random>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// #include "glm/ext.hpp"
// #include "glm/gtx/string_cast.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/quaternion.hpp>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#define INVALID_OGL_VALUE 0

#pragma comment (lib, "opengl32.lib" )
