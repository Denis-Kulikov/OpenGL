#include <GL/glew.h>
#include <GL/freeglut.h>

void draw (int i);
static void RenderSceneCB();
static void InitializeGlutCallbacks();
GLuint LoadShader(const GLchar** shader_code, GLuint type);
static void CompileShaders();
