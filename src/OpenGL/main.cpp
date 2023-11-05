
#include <iostream>
#include <cmath>
#include <cassert>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_3d.h"
#include "pipeline.hpp"


static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;                                                                \n\
                                                                                    \n\
out vec4 Color;                                                                     \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWorld * vec4(Position, 1.0);                                     \n\
    Color = vec4(clamp(Position, 0.0, 1.0), 1.0);                                   \n\
}";

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
in vec4 Color;                                                                      \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = Color;                                                              \n\
}";

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;
const int width = 1024;
const int height = 768;
const float radius = 0.8f;
bool flag_mr = false; 

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);


    static float Scale = 0.0f;

    Scale += 0.001f;

    pipeline p;
    // p.Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
    // p.WorldPos(sinf(Scale), 0.0f, 0.0f);
    // p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);

    p.Scale(0.5f, 0.5f, 0.5f);
    p.WorldPos(0.0f, -0.2f, 0.0f);
    p.Rotate(0.0f, Scale * 100.0f, 0.0f);

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetTransform());

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}


static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
}

static void CreateVertexBuffer()
{
    Vector3f Vertices[4];
    Vertices[0] = Vector3f(0.0f, -1.0f, -1.0f);
    Vertices[1] = Vector3f(-0.717f, -1.0f, 0.717f);
    Vertices[2] = Vector3f(0.717f, -1.0f, 0.717f);
    Vertices[3] = Vector3f(0.0f, 1.0f, 0.0f);

 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
    unsigned int Indices[] = { 0, 3, 1,
                               1, 3, 2,
                               2, 3, 0,
                               0, 2, 1 };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

GLuint LoadShader(const GLchar** shader_code, GLuint type)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, shader_code, NULL);
    glCompileShader(shader);

    GLint ok;
    GLchar log[2000];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(shader, 2000, NULL, log);
        printf("Shader: %s\n", log);
    }

    return shader;
}

static void CompileShaders()
{
    GLuint gScaleLocation;
    GLuint shader_color = LoadShader(&pFS, GL_FRAGMENT_SHADER);
    GLuint shader_position = LoadShader(&pVS, GL_VERTEX_SHADER);

    GLuint ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, shader_color);
    glAttachShader(ShaderProgram, shader_position);
    glLinkProgram(ShaderProgram);

    GLint ok;
    GLchar log[2000];
    glGetProgramiv(ShaderProgram, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(shader_color, 2000, NULL, log);
        std::cout << log << std::endl;
    }

    glUseProgram(ShaderProgram);

    gScaleLocation = glGetUniformLocation(ShaderProgram, "gWorld");
    assert(gScaleLocation != 0xFFFFFFFF);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);

    glutInitWindowSize(width, height);
    glScalef(height / width, 1, 1);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial");

    InitializeGlutCallbacks();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    CreateVertexBuffer();
    CreateIndexBuffer();

    CompileShaders();

    glutMainLoop();

    return 0;
}
