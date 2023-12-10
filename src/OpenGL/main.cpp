
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

#define NUMBER_BODY 1000
#define NUMBER_CLUSTER 3

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;
GLuint ShaderProgram;

const int width = 1024;
const int height = 768;
const float size = 0.0225f;
const float variance = 1e-10;
int iteration = 0;

int n = NUMBER_BODY;
int cn = NUMBER_CLUSTER;
struct cluster *p = (struct cluster*)calloc(sizeof(*p), n + cn);
struct distance_by_index *distances = (struct distance_by_index*)malloc(sizeof(*distances) * (n + cn));

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
    GLuint LengthLocation = glGetUniformLocation(ShaderProgram, "Cluster");
    glUniform1f(LengthLocation, p[i].cluster);
    glUniform3fv(FragPosLocation, 1, &p[i].pipeline.m_worldPos.x);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)pipeline.GetTrans());

    glPushMatrix();
    glutSolidSphere(1.0, 30, 30);
    glPopMatrix();
}

static void RenderSceneCB()
{
    iteration++;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(0);
    draw_cube();

    glUseProgram(ShaderProgram); 
    for (int i = 0; i < n + cn; i++) {
        int particleIndex = distances[i].index;
        draw(particleIndex);
    }

    glPopMatrix();
    glutSwapBuffers();

    // getchar();

    // for (int i = 0; i < cn; i++) {
        // p[n + i].pipeline.m_worldPos.Print();
    // }
    // cout << endl;
    float *prev_variance = calculate_variance(n, cn, p);
    assign_clusters(n, cn, p);
    move_cluster(n, cn, p);
    float *cur_variance = calculate_variance(n, cn, p);

    if ((fabs(prev_variance[0] - cur_variance[0]) < variance) && 
        (fabs(prev_variance[0] - cur_variance[0]) < variance) && 
        (fabs(prev_variance[0] - cur_variance[0]) < variance)) {
        std::cout << "Number of iterations: " << iteration << "\n";
        std::cout << "-------------------------\n";
        for (int i = 0; i < cn; i++) {
            std::cout << "Cluster " << i << " centroid: (" << p[n + i].pipeline.m_worldPos.x << ", " << p[n + i].pipeline.m_worldPos.y << ", " << p[n + i].pipeline.m_worldPos.z << ")\n";
            int points = 0;
            for (int j = 0; j < n; j++)
                if (p[j].cluster == i)
                    points++;

            std::cout << "Points in cluster " << i << ":\n";
            // for (const Point& p : points)
            //     if (p.cluster == static_cast<int>(i))
            //         std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ")\n";
            std::cout << "-------------------------\n";
        }
        getchar();
        free(prev_variance);
        free(cur_variance);
        glutLeaveMainLoop();
    }

    free(prev_variance);
    free(cur_variance);

    for (int i = 0; i < 100*1000*1000; i++);
}


static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
}

// static void CreateVertexBuffer()
// {
//     float cube_size = 0.5;

//     Vector3f Vertices[8];
//     Vertices[0] = Vector3f(-cube_size, cube_size, -cube_size);
//     Vertices[1] = Vector3f(cube_size,  cube_size,  -cube_size);
//     Vertices[2] = Vector3f(cube_size, -cube_size, -cube_size);
//     Vertices[3] = Vector3f(-cube_size, -cube_size, -cube_size);
//     Vertices[4] = Vector3f(-cube_size, cube_size,  cube_size);
//     Vertices[5] = Vector3f(cube_size,  cube_size,  cube_size);
//     Vertices[6] = Vector3f(cube_size, -cube_size,  cube_size);
//     Vertices[7] = Vector3f(-cube_size, -cube_size, cube_size);

//  	glGenBuffers(1, &VBO);
// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
// }

// static void CreateIndexBuffer()
// {
//     unsigned int Indices[] = {
//         0, 1, 2, 3, // front
//         0, 1, 4, 5, // top
//         0, 2, 4, 6, // left
//         1, 3, 5, 7, // right
//         2, 3, 6, 7, // bot
//         4, 5, 6, 7  // back
//     };

//     glGenBuffers(1, &IBO);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
// }

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
    glutInitWindowPosition(100, 200);
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

    init_points(n, cn, size, p, distances);
    qsort(distances, n + cn, sizeof(*distances), CompareParticleDistances);
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
