#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

static void CreateVertexBuffer();
static void CreateIndexBuffer();
GLuint LoadShader(const GLchar** shader_code, GLuint type);
static void CompileShaders();