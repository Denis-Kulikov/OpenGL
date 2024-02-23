#include "math_3d.hpp"
#include "object_transform.hpp"

class sprite 
{
public:
    std::string name;
    ObjectTransform trans;
    vector3f color;
    GLuint texture;
    GLuint shader;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    GLuint loadShader(const char *shader_path, GLuint type);
    GLuint compileShaders();
    GLuint loadTexures(const char *texture_path);
    void initializeGeometry();
    void draw();

private:
    GLuint loadShader(const char *shader_path, GLuint type);
    void compileShadersProgram(GLuint ShaderProgram, const char *FS, const char *VS);

};
