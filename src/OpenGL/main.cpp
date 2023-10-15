
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_3d.h"


static const GLchar* pVS = "#version 110                                      \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    gl_Position = gl_Vertex;  \n\
}";

static const GLchar* pFS = "#version 110                                      \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);                                  \n\
}";

GLuint VBO;
int width = 1024;
int height = 768;


static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}


static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
}

static void CreateVertexBuffer()
{
    // Vector3f Vertices[1];
    // Vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);

    Vector3f Vertices[3];
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
    Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
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
    GLuint shader_color = LoadShader(&pFS, GL_FRAGMENT_SHADER);
    GLuint shader_position = LoadShader(&pVS, GL_VERTEX_SHADER);

    GLuint prog = glCreateProgram();
    glAttachShader(prog, shader_color);
    glAttachShader(prog, shader_position);
    glLinkProgram(prog);

    GLint ok;
    GLchar log[2000];
    glGetShaderiv(prog, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(shader_color, 2000, NULL, log);
        std::cout << log << std::endl;
    }

    glUseProgram(prog);
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

    GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    CreateVertexBuffer();

    CompileShaders();

    glutMainLoop();

    return 0;
}
