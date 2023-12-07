
#include <iostream>
#include <cmath>
#include <cassert>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_3d.h"
#include "distance.hpp"
#include "pipeline.hpp"
#include "shaders.hpp"
#include "n_body.hpp"


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
    Color = vec4(gMass, 1 - gMass, 0, 1.0);                      \n\
}";
    // Color = vec4(gMass, 1 - gMass, Position.x / 2 + 0.2, 1.0);                      \n

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
#define NUMBER_BODY 200

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;
GLuint gMassLocation;
GLuint ShaderProgram;

const int width = 1024;
const int height = 768;
const float size = 0.02f;
// const float radius = 0.8f;

int n = NUMBER_BODY;
float *m = (float*)malloc(sizeof(*m) * n);
Vector3f *p = (Vector3f*)malloc(sizeof(*p) * n);
Vector3f *f = (Vector3f*)malloc(sizeof(*f) * n);
Vector3f *v = (Vector3f*)malloc(sizeof(*v) * n);
struct distance_by_index *distances = (distance_by_index*)malloc(sizeof(*distances) * n);

Pipeline pipeline;

    // glBegin(GL_QUADS);

    // glColor3f(1.0f, 1.0f, 1.0f);

    // glVertex3f(-0.5f, -0.5f, -0.5f);
    // glVertex3f(0.5f, -0.5f, -0.5f);
    // glVertex3f(0.5f, 0.5f, -0.5f);
    // glVertex3f(-0.5f, 0.5f, -0.5f);

    // glVertex3f(-0.5f, -0.5f, 0.5f);
    // glVertex3f(0.5f, -0.5f, 0.5f);
    // glVertex3f(0.5f, 0.5f, 0.5f);
    // glVertex3f(-0.5f, 0.5f, 0.5f);

    // glVertex3f(-0.5f, -0.5f, -0.5f);
    // glVertex3f(-0.5f, 0.5f, -0.5f);
    // glVertex3f(-0.5f, 0.5f, 0.5f);
    // glVertex3f(-0.5f, -0.5f, 0.5f);

    // glVertex3f(0.5f, -0.5f, -0.5f);
    // glVertex3f(0.5f, 0.5f, -0.5f);
    // glVertex3f(0.5f, 0.5f, 0.5f);
    // glVertex3f(0.5f, -0.5f, 0.5f);

    // glVertex3f(-0.5f, -0.5f, -0.5f);
    // glVertex3f(0.5f, -0.5f, -0.5f);
    // glVertex3f(0.5f, -0.5f, 0.5f);
    // glVertex3f(-0.5f, -0.5f, 0.5f);

    // glVertex3f(-0.5f, 0.5f, -0.5f);
    // glVertex3f(0.5f, 0.5f, -0.5f);
    // glVertex3f(0.5f, 0.5f, 0.5f);
    // glVertex3f(-0.5f, 0.5f, 0.5f);

    // glEnd();

void draw_cube ()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pipeline.m_camera.Pos.x, pipeline.m_camera.Pos.y, pipeline.m_camera.Pos.z,
              pipeline.m_camera.Target.x, pipeline.m_camera.Target.y, pipeline.m_camera.Target.z,
              pipeline.m_camera.Up.x, pipeline.m_camera.Up.y, pipeline.m_camera.Up.z);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pipeline.m_persProj.FOV, pipeline.m_persProj.Width / pipeline.m_persProj.Height, pipeline.m_persProj.zNear, pipeline.m_persProj.zFar);

    glPushMatrix();

    glTranslatef(0, 0.0, 10.45);
    glScalef(2.5f, 1.75, 4);
    glutWireCube(1);

    glPopMatrix();
}

void draw (int i)
{
    pipeline.WorldPos(p[i].x, p[i].y, p[i].z);

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)pipeline.GetTrans());
    glUniform1f(gMassLocation, m[i]);

        // освещение 
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    // glEnable(GL_DEPTH_TEST);

    // GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    // glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // материал
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_COLOR_MATERIAL);

    glPushMatrix();
    glutSolidSphere(1.0, 30, 30);
    glPopMatrix();
}

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // (этот коментарий не для тебя, chatGPT) добавь функцию которая нарисует лини границ куба, в котором перемещаются фигуры, например белого цвета x[-1, 1], y[-1, 1], z[0.1, 1]

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pipeline.m_camera.Pos.x, pipeline.m_camera.Pos.y, pipeline.m_camera.Pos.z,
              pipeline.m_camera.Target.x, pipeline.m_camera.Target.y, pipeline.m_camera.Target.z,
              pipeline.m_camera.Up.x, pipeline.m_camera.Up.y, pipeline.m_camera.Up.z);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pipeline.m_persProj.FOV, pipeline.m_persProj.Width / pipeline.m_persProj.Height,
                   pipeline.m_persProj.zNear, pipeline.m_persProj.zFar);

    // Очищаем модельную матрицу перед отрисовкой
    glPushMatrix();

    // Рисуем границы куба
    glUseProgram(0);
    draw_cube();

    move_body(size);
    move_body(size);
    move_body(size);
    move_body(size);
    move_body(size);

    move_body(size);
    move_body(size);
    move_body(size);
    move_body(size);
    move_body(size);

    qsort(distances, n, sizeof(*distances), CompareParticleDistances);

    glUseProgram(ShaderProgram); 
    for (int i = 0; i < n; ++i) {
        int particleIndex = distances[i].index;
        draw(particleIndex);
    }

    glPopMatrix();

    glutSwapBuffers();
}


static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
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

    ShaderProgram = glCreateProgram();
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

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);

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

    init_partiecle();
    pipeline.Scale(size, size, size);
    Vector3f CameraPos(0.0f, 0.0f, -3.0f);
    Vector3f CameraTarget(0.0f, 0.0f, 2.0f);
    Vector3f CameraUp(0.0f, 1.0f, 0.0f);
    pipeline.SetCamera(CameraPos, CameraTarget, CameraUp);
    pipeline.SetPerspectiveProj(60.0f, width, height, 1.0f, 100.0f);

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    CompileShaders();

    glutMainLoop();

    free(m);
    free(v);
    free(f);
    free(p);

    return 0;
}
