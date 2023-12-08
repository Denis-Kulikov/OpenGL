
#include <iostream>
#include <cmath>
#include <cassert>
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
    Color = vec4(gMass, 1 - gMass, 0, 1.0);                                                     \n\
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

#define NUMBER_BODY 100

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;
GLuint gMassLocation;
GLuint ShaderProgram;

const int width = 1024;
const int height = 768;
const float size = 0.1f;

int n = NUMBER_BODY;
struct cluster *p = (struct cluster*)calloc(sizeof(*p), n);
struct distance_by_index *distances = (struct distance_by_index*)malloc(sizeof(*distances) * n);

Pipeline pipeline;

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

void computeSphereNormals(float radius, int numSlices, int numStacks, std::vector<Vector3f>& normals)
{
    normals.clear();

    for (int i = 0; i <= numStacks; ++i) {
        float phi = M_PI * static_cast<float>(i) / numStacks;
        for (int j = 0; j <= numSlices; ++j) {
            float theta = 2.0f * M_PI * static_cast<float>(j) / numSlices;

            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi);
            float z = radius * sin(phi) * sin(theta);

            normals.push_back(Vector3f(x, y, z).Normalize());
        }
    }
}

void draw(int i)
{
    pipeline.WorldPos(p[i].pipeline.m_worldPos.x, p[i].pipeline.m_worldPos.y, p[i].pipeline.m_worldPos.z);

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)pipeline.GetTrans());
    glUniform1f(gMassLocation, p[i].pipeline.m_worldPos.z / 4);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPos[] = {0.0f, 0.0f, -1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Material properties
    GLfloat matAmbient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat matDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat matShininess[] = {100.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // Новый код для нормалей и вершин
    glEnableVertexAttribArray(0);  // Атрибут для вершин
    glEnableVertexAttribArray(1);  // Атрибут для нормалей

    std::vector<Vector3f> sphereNormals;
    computeSphereNormals(1.0f, 30, 30, sphereNormals);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);  // Атрибут вершин
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, &sphereNormals[0]);  // Атрибут нормалей

    glPushMatrix();
    glScalef(1.0f, 1.0f, 1.0f);
    glutSolidSphere(1.0, 30, 30);
    glPopMatrix();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
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

    init_pos(n, size, p, distances);
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

    glutMainLoop();

    free(p);

    return 0;
}
