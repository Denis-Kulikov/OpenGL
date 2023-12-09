
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

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

#include "math_3d.h"
#include "cluster.hpp"
#include "distance.hpp"
#include "pipeline.hpp"
#include "shaders.hpp"

#define NUMBER_BODY 100

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;
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

void draw(int i)
{
    pipeline.WorldPos(p[i].pipeline.m_worldPos.x, p[i].pipeline.m_worldPos.y, p[i].pipeline.m_worldPos.z);
    GLuint FragPosLocation = glGetUniformLocation(ShaderProgram, "FragPos");

    // В вершинном шейдере
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)pipeline.GetTrans());
    glUniform3fv(FragPosLocation, 1, &p[i].pipeline.m_worldPos.x);

    GLuint lightPosLocation = glGetUniformLocation(ShaderProgram, "lightPosition");
    const Vector3f lightPosition(0.0f, 1.0f, 0.0f);

    // Переменные для материала
    const Vector3f materialAmbient(0.7f, 0.7f, 0.7f);
    const Vector3f materialDiffuse(0.8f, 0.8f, 0.8f);
    const Vector3f materialSpecular(1.0f, 1.0f, 1.0f);
    float materialShininess = 100.0f;

    // Получение локаций uniform-переменных для материала
    GLuint materialAmbientLocation = glGetUniformLocation(ShaderProgram, "materialAmbient");
    GLuint materialDiffuseLocation = glGetUniformLocation(ShaderProgram, "materialDiffuse");
    GLuint materialSpecularLocation = glGetUniformLocation(ShaderProgram, "materialSpecular");
    GLuint materialShininessLocation = glGetUniformLocation(ShaderProgram, "materialShininess");

    // Во фрагментном шейдере
    glUniform3fv(lightPosLocation, 1, &lightPosition.x);
    glUniform3fv(materialAmbientLocation, 1, &materialAmbient.x);
    glUniform3fv(materialDiffuseLocation, 1, &materialDiffuse.x);
    glUniform3fv(materialSpecularLocation, 1, &materialSpecular.x); 
    glUniform1f(materialShininessLocation, materialShininess);

    glPushMatrix();
    glutSolidSphere(1.0, 30, 30);
    glPopMatrix();
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

    glutKeyboardFunc(KeyboardCB);
    glutMainLoop();

    free(p);

    return 0;
}
