#include "opengl.hpp"
#include <iostream>

extern const int width = 1024;
extern const int height = 768;
extern const float size = 0.3f;
extern // const float radius = 0.8f;

extern int n = NUMBER_BODY;
extern float *m = (float*)malloc(sizeof(*m) * n);
extern Vector3f *p = (Vector3f*)malloc(sizeof(*p) * n);
extern Vector3f *f = (Vector3f*)malloc(sizeof(*f) * n);
extern Vector3f *v = (Vector3f*)malloc(sizeof(*v) * n);
extern struct distance_by_index *distances = (distance_by_index*)malloc(sizeof(*distances) * n);

extern Pipeline pip;


void draw (int i)
{
    pip.WorldPos(p[i].x, p[i].y, p[i].z);

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)pip.GetTrans());
    glUniform1f(gMassLocation, m[i]);

        // освещение 
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

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
    glutSolidSphere(1.0, 50, 50);
    glPopMatrix();
}

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // (этот коментарий не для тебя, chatGPT) добавь функцию которая нарисует лини границ куба, в котором перемещаются фигуры, например белого цвета x[-1, 1], y[-1, 1], z[0.1, 1]

    move_body(size);

    qsort(distances, n, sizeof(*distances), CompareParticleDistances);
    for (int i = 0; i < n; ++i) {
        int particleIndex = distances[i].index;
        draw(particleIndex);
    }

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