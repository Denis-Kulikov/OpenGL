#include "pipeline.hpp"

#define PI = 3.14159265359f;

bool RenderSceneCB();
GLuint LoadTexures(const char *texture_path);
void CompileShaders();
void InitializeGLFW(GLFWwindow* &window);
