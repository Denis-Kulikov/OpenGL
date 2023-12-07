
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
#include "lib.hpp"


static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;                                                                \n\
uniform float gMass;                                                                \n\
                                                                                    \n\
out vec4 Color;                                                                     \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWorld * vec4(Position, 1.0);                                     \n\
    Color = vec4(gMass, 1 - gMass, Position.x / 2 + 0.2, 1.0);                                   \n\
}";
    // Color = vec4(Position.x / 2 + 0.2, Position.y / 2 + 0.2 + gMass * 100, Position.z / 2 + 0.2, 1.0);                                   \n
    // Color = vec4(clamp(Position, 0.0, 1.0) / 2 + 0.4, 1.0);                                   \n

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

#define NUMBER_VERTICES 14
#define NUMBER_BODY 5

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;
GLuint gMassLocation;

const int width = 1024;
const int height = 768;
const float radius = 0.8f;
bool flag_mr = false; 

int n = NUMBER_BODY;
float *m = (float*)malloc(sizeof(*m) * n);
Vector3f *p = (Vector3f*)malloc(sizeof(*p) * n);
Vector3f *f = (Vector3f*)malloc(sizeof(*f) * n);
Vector3f *v = (Vector3f*)malloc(sizeof(*v) * n);

Pipeline pip;

static void CreateVertexBuffer()
{
    Vector3f Vertices[NUMBER_VERTICES];
    Vertices[0] = Vector3f(0.0, 1.0, 0.0);
    Vertices[1] = Vector3f(0.707, 0.707, 0.0);
    Vertices[2] = Vector3f(1.0, 0.0, 0.0);
    Vertices[3] = Vector3f(0.707, -0.707, 0.0);
    Vertices[4] = Vector3f(0.0, -1.0, 0.0);
    Vertices[5] = Vector3f(-0.707, -0.707, 0.0);
    Vertices[6] = Vector3f(-1.0, 0.0, 0.0);
    Vertices[7] = Vector3f(-0.707, 0.707, 0.0);
    Vertices[8] = Vector3f(0.0, 0.0, 1.0);
    Vertices[9] = Vector3f(0.0, 0.0, -1.0);
    Vertices[10] = Vector3f(0.0, 0.707, -0.707);
    Vertices[11] = Vector3f(0.0, -0.707, -0.707);
    Vertices[12] = Vector3f(0.0, 0.707, 0.707);
    Vertices[13] = Vector3f(0.0, -0.707, 0.707);

 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
    unsigned int Indices[] = {
        0, 1, 12,
        0, 7, 12,
        6, 7, 12,
        6, 8, 12,
        6, 8, 13,
        5, 6, 13,
        4, 5, 13,
        3, 4, 13,
        2, 3, 13,
        2, 8, 13,
        2, 8, 12,
        1, 2, 12,

        0, 10, 1,
        0, 7, 10,
        6, 7, 10,
        6, 9, 10,
        6, 9, 11,
        5, 6, 11,
        4, 5, 11,
        3, 4, 11,
        2, 3, 11,
        2, 9, 11,
        2, 9, 10,
        2, 10, 1,
        1, 2, 10
    };

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
    gMassLocation = glGetUniformLocation(ShaderProgram, "gMass");
    assert(gScaleLocation != 0xFFFFFFFF);
}