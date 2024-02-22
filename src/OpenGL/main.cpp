
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
#include <GL/freeglut.h>

#include "math_3d.h"
#include "distance.hpp"
#include "n_body.hpp"
#include "pipeline.hpp"
#include "shaders.hpp"

#define NUMBER_BODY 100

using namespace std;

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;
GLuint gMassLocation;
GLuint ShaderSphere;
GLuint ShaderCube;

const int width = 1600;
const int height = 900;
const float size = 0.075f;

int n = NUMBER_BODY;
float *m = (float*)malloc(sizeof(*m) * n);
Vector3f *p = (Vector3f*)malloc(sizeof(*p) * n);
Vector3f *f = (Vector3f*)malloc(sizeof(*f) * n);
Vector3f *v = (Vector3f*)malloc(sizeof(*v) * n);
struct distance_by_index *distances = (distance_by_index*)malloc(sizeof(*distances) * n);

Pipeline pipeline;

void DrawCube()
{
    float width_space = 2.5f;
    float hight_space = 1.75f;
    float length_space = 4.0f;

    pipeline.WorldPos(0, 0, 2);
    pipeline.Scale(width_space * 2.0f, hight_space * 2.0f, length_space);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)pipeline.GetTrans());

    glutWireCube(1);

    pipeline.Scale(size, size, size);
}

void DrawSphere(int i)
{
    pipeline.WorldPos(p[i].x, p[i].y, p[i].z);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)pipeline.GetTrans());
    glUniform1f(gMassLocation, m[i]);

    glutSolidSphere(1.0, 30, 30);
}

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(ShaderCube);
    DrawCube();

    move_body(size);
    move_body(size);
    move_body(size);

    glUseProgram(ShaderSphere); 
    for (int i = 0; i < n; i++) {
        int particleIndex = distances[i].index;
        DrawSphere(particleIndex);
    }

    glutSwapBuffers();
}

void PushMatrix(Pipeline &pipe)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pipe.m_camera.Pos.x, pipe.m_camera.Pos.y, pipe.m_camera.Pos.z,
              pipe.m_camera.Target.x, pipe.m_camera.Target.y, pipe.m_camera.Target.z,
              pipe.m_camera.Up.x, pipe.m_camera.Up.y, pipe.m_camera.Up.z);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pipe.m_persProj.FOV, pipe.m_persProj.Width / pipe.m_persProj.Height,
                   pipe.m_persProj.zNear, pipe.m_persProj.zFar);

    glPushMatrix();
}

static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
}

GLuint LoadShader(const char *shader_path, GLuint type)
{
    ifstream shader_file(shader_path);

    if (!shader_file.is_open()) {
        cerr << "Error: Could not open shader file '" << shader_path << "'" << endl;
        return 0;
    }

    stringstream shader_stream;
    shader_stream << shader_file.rdbuf();
    shader_file.close();

    char* code = (char*)malloc(shader_stream.str().length() + 1);
    for (size_t i = 0; i < shader_stream.str().length(); ++i) {
        code[i] = shader_stream.str()[i];
    }
    code[shader_stream.str().length()] = '\0';
    const GLchar* shader_code = code;

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);

    GLint ok;
    GLchar log[2000];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(shader, 2000, NULL, log);
        printf("Shader(%s): %s\n", shader_path, log);
        cout << shader_stream.str().c_str() << endl;
    }

    return shader;
}

void CompileShadersCube()
{
    GLuint gScaleLocation;

    GLuint shader_position = LoadShader("shaders/cube_vs.glsl", GL_VERTEX_SHADER);

    ShaderCube = glCreateProgram();
    glAttachShader(ShaderCube, shader_position);
    glLinkProgram(ShaderCube);

    GLint ok;
    GLchar log[2000];
    glGetProgramiv(ShaderCube, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(ShaderCube, 2000, NULL, log);
        std::cout << "ShaderCube Compilation Log:\n" << log << std::endl;
    
        GLint infoLogLength;
        glGetShaderiv(shader_position, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader_position, infoLogLength, NULL, infoLog);
        std::cout << "ShaderCube shader_position Log:\n" << infoLog << std::endl;
        delete[] infoLog;
    }

    glUseProgram(ShaderCube);

    gScaleLocation = glGetUniformLocation(ShaderCube, "gWorld");
    assert(gScaleLocation != 0xFFFFFFFF);
}

void CompileShadersSphere()
{
    GLuint gScaleLocation;

    GLuint shader_color = LoadShader("shaders/sphere_fs.glsl", GL_FRAGMENT_SHADER);
    GLuint shader_position = LoadShader("shaders/sphere_vs.glsl", GL_VERTEX_SHADER);

    ShaderSphere = glCreateProgram();
    glAttachShader(ShaderSphere, shader_color);
    glAttachShader(ShaderSphere, shader_position);
    glLinkProgram(ShaderSphere);

    GLint ok;
    GLchar log[2000];
    glGetProgramiv(ShaderSphere, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(ShaderSphere, 2000, NULL, log);
        std::cout << "ShaderSphere Compilation Log:\n" << log << std::endl;
    
        GLint infoLogLength;
        glGetShaderiv(shader_color, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader_color, infoLogLength, NULL, infoLog);
        std::cout << "ShaderSphere shader_color Log:\n" << infoLog << std::endl;
        delete[] infoLog;
    }

    glUseProgram(ShaderSphere);

    gScaleLocation = glGetUniformLocation(ShaderSphere, "gWorld");
    gMassLocation = glGetUniformLocation(ShaderSphere, "gMass");
    assert(gScaleLocation != 0xFFFFFFFF);
}

void CompileShaders()
{
    CompileShadersSphere();
    CompileShadersCube();
}

static void KeyboardCB(unsigned char Key, int x, int y)
{
    double speed = 0.075;
    switch (Key) {
    case 'f':
        glutLeaveMainLoop();
        break;
    case 'w':
        pipeline.m_camera.Pos.z += speed;
        break;
    case 's':
        pipeline.m_camera.Pos.z -= speed;
        break;
    case 'd':
        pipeline.m_camera.Pos.x += speed;
        break;
    case 'a':
        pipeline.m_camera.Pos.x -= speed;
        break;
    case ' ':
        pipeline.m_camera.Pos.y += speed;
        break;
    case 'c':
        pipeline.m_camera.Pos.y -= speed;
        break;
    case 'e':
        pipeline.m_camera.Target.x += speed;
        break;
    case 'q':
        pipeline.m_camera.Target.x -= speed;
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);

    glutInitWindowSize(width, height);
    glScalef(height / width, 1, 1);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("KMeans");

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

    pipeline.Scale(size, size, size);
    Vector3f CameraPos(0.0f, 0.5f, -5.0f);
    Vector3f CameraTarget(0.0f, 0.0f, 2.0f);
    Vector3f CameraUp(0.0f, 1.0f, 0.0f);
    pipeline.Scale(size, size, size);
    pipeline.SetCamera(CameraPos, CameraTarget, CameraUp);
    pipeline.SetPerspectiveProj(60.0f, width, height, 1.0f, 100.0f);
    PushMatrix(pipeline);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    CompileShaders();
    init_partiecle();

    glutKeyboardFunc(KeyboardCB);
    glutMainLoop();

    glPopMatrix();

    free(m);
    free(v);
    free(f);
    free(p);

    return 0;
}
