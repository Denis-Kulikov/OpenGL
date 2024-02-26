#include <glfw.hpp>
#include <sprite.hpp>
#include <try.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void sprite::loadTexures(const char *texturePath)
{
    if (texturePath == nullptr) return;

    int x, y, n;
    std::string path = std::string("assets/") + texturePath;
    unsigned char *img = stbi_load(path.c_str(), &x, &y, &n, 0);

    TRY(img == nullptr, std::string("Failed to load texture: " + path))

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, (n == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    gTextureSamplerLocation = glGetUniformLocation(shader, "textureSampler");

    stbi_image_free(img);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
}

GLuint sprite::loadShader(const char *shaderPath, GLuint type)
{
    std::ifstream shaderFile(shaderPath);
    if (!shaderFile.is_open()) {
        std::cerr << "Error: Could not open shader file '" << shaderPath << "'" << std::endl;
        return 0;
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    std::string shaderCode = shaderStream.str();
    const GLchar* shaderCodePtr = shaderCode.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCodePtr, NULL);
    glCompileShader(shader);

    GLint ok;
    GLchar log[2000];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(shader, 2000, NULL, log);
        printf("Shader(%s): %s\n", shaderPath, log);
        std::cout << shaderCode << std::endl;
    }

    return shader;
}


void sprite::compileShaders(const char *FS, const char *VS)
{
    if (shader == 0) 
        shader = glCreateProgram();

    GLuint fragmentShader;
    GLuint vertexShader;

    if (FS != nullptr) {
        fragmentShader = loadShader(FS, GL_FRAGMENT_SHADER);
        glAttachShader(shader, fragmentShader);
    }
    if (VS != nullptr) {
        vertexShader = loadShader(VS, GL_VERTEX_SHADER);
        glAttachShader(shader, vertexShader);
    }

    glLinkProgram(shader);

    GLint ok;
    GLchar log[2000];
    glGetProgramiv(shader, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(shader, 2000, NULL, log);
        std::cout << "Shader " << name << " compilation Log:\n" << log << std::endl;
    
        if (FS != nullptr) {
            GLint infoLogLength;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar *infoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, infoLog);
            std::cout << "Shader fragmentShader Log:\n" << infoLog << std::endl;
            delete[] infoLog;
        }
        if (VS != nullptr) {
            GLint infoLogLength;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar *infoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(vertexShader, infoLogLength, NULL, infoLog);
            std::cout << "Shader vertexShader Log:\n" << infoLog << std::endl;
            delete[] infoLog;
        }
        std::cout << std::endl;
    }

    gWorldLocation = glGetUniformLocation(shader, "gWorld");
    gColorLocation = glGetUniformLocation(shader, "gColor"); // assert(gColorLocation != 0xFFFFFFFF);
    assert(gWorldLocation != 0xFFFFFFFF);
}

void sprite::initializeGeometry()
{
    std::vector<GLfloat> vertices = {
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,  1.0f, 0.0f
    };

    std::vector<GLuint> indices = {
        0, 1, 2,
        0, 2, 3
    };

    numVertices = vertices.size();
    numIndices = indices.size();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

sprite::sprite(const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath)
{
    trans.SetTransform(_trans);

    compileShaders(FS, VS);
    loadTexures(texturePath);
}

sprite::sprite(const std::string &_name, const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath)
    : name(_name)
{
    trans.SetTransform(_trans);

    initializeGeometry();
    compileShaders(FS, VS);
    loadTexures(texturePath);
}