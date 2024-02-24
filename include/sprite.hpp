#include "glfw.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class sprite 
{
public:
    sprite(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> &_color, const char *FS, const char *VS, const char *texturePath);

    void initializeRenderingData();
    void loadTexures(const char *texturePath);
    void compileShaders(const char *FS, const char *VS);
    void initializeGeometry();

    std::string name;
    objectTransform trans;
    Vector3<GLfloat> color;

    GLuint texture;
    GLuint shader;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

private:
    GLuint loadShader(const char *shaderPath, GLuint type);
};
