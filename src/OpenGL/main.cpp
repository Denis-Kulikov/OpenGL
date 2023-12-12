
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

const int width = 1280;
const int height = 768;

float width_space   = 2.0f;
float hight_space   = 2.0f;
float length_space  = 2.0f;

int n = NUMBER_BODY;
const float size = 0.15f;

Pipeline pipeline;
struct distance_by_index *distances = (distance_by_index*)malloc(sizeof(*distances) * n);
Vector3f *p = (Vector3f*)malloc(sizeof(*p) * n);
Vector3f *f = (Vector3f*)malloc(sizeof(*f) * n);
Vector3f *v = (Vector3f*)malloc(sizeof(*v) * n);
float *m = (float*)malloc(sizeof(*m) * n);

void DrawCube()
{
    pipeline.object.SetWorldPos(0, 0, 0);
    pipeline.object.SetRotate(0, 0, 0);
    pipeline.object.SetScale(width_space * 2.0f, hight_space * 2.0f, length_space * 2.0f);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)pipeline.GetTrans());

    glutWireCube(1.0f);
}

void DrawSphere(int i)
{
    pipeline.object.SetWorldPos(p[i].x, p[i].y, p[i].z);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)pipeline.GetTrans());
    glUniform1f(gMassLocation, m[i]);

    glutSolidSphere(1.0f, 30, 30);
}

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(ShaderCube);
    DrawCube();

    move_body(size);

    qsort(distances, n, sizeof(*distances), CompareParticleDistances);
    glUseProgram(ShaderSphere); 
    pipeline.object.SetScale(size, size, size);
    pipeline.object.SetRotate(0, 0, 0);
    for (int i = 0; i < n; i++) {
        int particleIndex = distances[i].index;
        DrawSphere(particleIndex);
    }

    glutSwapBuffers();
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
    double speed_movement = 0.125;
    double speed_rotation = 0.075;
    switch (Key) {
    case 'f':
        glutLeaveMainLoop();
        break;
    case 'w':
        pipeline.camera.Params.WorldPos.z += speed_movement;
        break;
    case 's':
        pipeline.camera.Params.WorldPos.z -= speed_movement;
        break;
    case 'd':
        pipeline.camera.Params.WorldPos.x += speed_movement;
        break;
    case 'a':
        pipeline.camera.Params.WorldPos.x -= speed_movement;
        break;
    case ' ':
        pipeline.camera.Params.WorldPos.y += speed_movement;
        break;
    case 'c':
        pipeline.camera.Params.WorldPos.y -= speed_movement;
        break;
    case 'e':
        pipeline.camera.Params.Target.x += speed_rotation;
        break;
    case 'q':
        pipeline.camera.Params.Target.x -= speed_rotation;
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
    glutCreateWindow("NBody");

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

    Vector3f CameraPos(0.0f, 0.1f, -7.0f);
    Vector3f CameraTarget(0.0f, 0.0f, 1.0f);
    Vector3f CameraUp(0.0f, 1.0f, 0.0f);
    pipeline.camera.SetCamera(CameraPos, CameraTarget, CameraUp);
    pipeline.camera.SetPerspectiveProj(60.0f, width, height, 0.5f, 100.0f);
    pipeline.object.SetScale(size, size, size);

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    CompileShaders();
    init_partiecle();

    glutKeyboardFunc(KeyboardCB);
    glutMainLoop();

    free(m);
    free(v);
    free(f);
    free(p);

    return 0;
}
