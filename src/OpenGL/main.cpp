
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
#include "cluster.hpp"
#include "distance.hpp"
#include "pipeline.hpp"
#include "shaders.hpp"

#define NUMBER_BODY 100
#define NUMBER_CLUSTER 3

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;
GLuint ShaderProgram;

const int width = 1024;
const int height = 768;
const float size = 0.075f;

int n = NUMBER_BODY;
int cn = NUMBER_CLUSTER;
struct cluster *p = (struct cluster*)calloc(sizeof(*p), n + cn);
struct distance_by_index *distances = (struct distance_by_index*)malloc(sizeof(*distances) * (n + cn));

Pipeline pipeline;

void draw_clusters(int i)
{
    pipeline.WorldPos(p[i].pipeline.m_worldPos.x, p[i].pipeline.m_worldPos.y, p[i].pipeline.m_worldPos.z);
    GLuint FragPosLocation = glGetUniformLocation(ShaderProgram, "FragPos");
    GLuint LengthLocation = glGetUniformLocation(ShaderProgram, "Cluster");
    glUniform1f(LengthLocation, 3);
    glUniform3fv(FragPosLocation, 1, &p[i].pipeline.m_worldPos.x);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)pipeline.GetTrans());

    glPushMatrix();
    glutSolidSphere(1.0, 30, 30);
    glPopMatrix();
}

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

void draw(int i)
{
    pipeline.WorldPos(p[i].pipeline.m_worldPos.x, p[i].pipeline.m_worldPos.y, p[i].pipeline.m_worldPos.z);
    GLuint FragPosLocation = glGetUniformLocation(ShaderProgram, "FragPos");
    GLuint LengthLocation = glGetUniformLocation(ShaderProgram, "Cluster");
    glUniform1f(LengthLocation, 0);
    glUniform3fv(FragPosLocation, 1, &p[i].pipeline.m_worldPos.x);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)pipeline.GetTrans());

    glPushMatrix();
    glutSolidSphere(1.0, 30, 30);
    glPopMatrix();
}

void calculate_distances() {
    for (int i = 0; i < n + cn; ++i) {
        distances[i].index = i;
        distances[i].dist = p[i].pipeline.m_worldPos.distance(Vector3f(0.0f, 0.0f, 0.0f));
    }
}


static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pipeline.m_camera.Pos.x, pipeline.m_camera.Pos.y, pipeline.m_camera.Pos.z,
              pipeline.m_camera.Target.x, pipeline.m_camera.Target.y, pipeline.m_camera.Target.z,
              pipeline.m_camera.Up.x, pipeline.m_camera.Up.y, pipeline.m_camera.Up.z);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pipeline.m_persProj.FOV, pipeline.m_persProj.Width / pipeline.m_persProj.Height,
                   pipeline.m_persProj.zNear, pipeline.m_persProj.zFar);

    glPushMatrix();

    glUseProgram(0);
    draw_cube();

    calculate_distances();
    qsort(distances, n + cn, sizeof(*distances), CompareParticleDistances);

    glUseProgram(ShaderProgram); 
    for (int i = 0; i < n + cn; i++) {
        int particleIndex = distances[i].index;
        if (p[particleIndex].is_cluster) {
            draw_clusters(particleIndex);
        } else {
            draw(particleIndex);
        }
    }

    glPopMatrix();

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
        printf("Shader: %s\n", log);
        cout << shader_stream.str().c_str() << endl;
    }

    return shader;
}

static void CompileShaders()
{
    GLuint gScaleLocation;

    GLuint shader_color = LoadShader("shaders/point_fs.glsl", GL_FRAGMENT_SHADER);
    GLuint shader_position = LoadShader("shaders/point_vs.glsl", GL_VERTEX_SHADER);

    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, shader_color);
    glAttachShader(ShaderProgram, shader_position);
    glLinkProgram(ShaderProgram);

    GLint ok;
    GLchar log[2000];
    glGetProgramiv(ShaderProgram, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(shader_color, 2000, NULL, log);
        std::cout << "shader_color:" << log << std::endl;
    
        GLint infoLogLength;
        glGetShaderiv(shader_color, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader_color, infoLogLength, NULL, infoLog);
        std::cout << "Shader Compilation Log:\n" << infoLog << std::endl;
        delete[] infoLog;
    }

    glUseProgram(ShaderProgram);

    gScaleLocation = glGetUniformLocation(ShaderProgram, "gWorld");
    assert(gScaleLocation != 0xFFFFFFFF);
}

static void KeyboardCB(unsigned char Key, int x, int y)
{
    switch (Key) {
        case 'q':
            glutLeaveMainLoop();
    }
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

    init_points(n, cn, size, p, distances);
    Vector3f CameraPos(0.0f, 0.05f, -3.0f);
    Vector3f CameraTarget(0.0f, 0.0f, 2.0f);
    Vector3f CameraUp(0.0f, 1.0f, 0.0f);
    pipeline.Scale(size, size, size);
    pipeline.SetCamera(CameraPos, CameraTarget, CameraUp);
    pipeline.SetPerspectiveProj(60.0f, width, height, 1.0f, 100.0f);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    CompileShaders();

    glutKeyboardFunc(KeyboardCB);
    glutMainLoop();

    free(p);

    return 0;
}
