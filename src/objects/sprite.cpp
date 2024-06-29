#include <render/glfw.hpp>
#include <object/sprite.hpp>
#include <lib-project/try.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

struct GeometryInfo Sprite::geometryInfo = {0, 0, 0, 0, 0};

void Sprite::loadTextures(const char *texturePath)
{
    if (texturePath == nullptr) return;

    int x, y, n;
    std::string path = std::string("assets/") + texturePath;
    unsigned char *img = stbi_load(path.c_str(), &x, &y, &n, 0);

    TRY(img == nullptr, std::string("Failed to load texture: " + path))

    // Найти ближайшие значения, кратные степени 2
    int new_x = 1 << (int)std::ceil(std::log2(x));
    int new_y = 1 << (int)std::ceil(std::log2(y));

    // Выделить память для измененного изображения
    unsigned char *resized_img = (unsigned char*)malloc(new_x * new_y * n);
    if (resized_img == nullptr) {
        std::cerr << "Failed to allocate memory for resized texture" << std::endl;
        stbi_image_free(img);
        return;
    }

    // Изменить размер изображения
    stbir_resize_uint8(img, x, y, 0, resized_img, new_x, new_y, 0, n);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Проверка формата изображения
    GLenum format = (n == 4) ? GL_RGBA : GL_RGB;

    // Загрузка текстуры в OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, format, new_x, new_y, 0, format, GL_UNSIGNED_BYTE, resized_img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    gTextureSamplerLocation = glGetUniformLocation(shader, "textureSampler");

    stbi_image_free(img);
    free(resized_img);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLfloat scale_x = static_cast<GLfloat>(x) / static_cast<GLfloat>(new_x);
    GLfloat scale_y = static_cast<GLfloat>(y) / static_cast<GLfloat>(new_y);
    Scale.VSet(1, static_cast<GLfloat>(new_y * scale_y) / static_cast<GLfloat>(new_x * scale_x), 0.0);
    // Устанавливаем правильное соотношение сторон на основе измененных размеров
    // Scale.VSet(1.0, static_cast<GLfloat>(new_y) / static_cast<GLfloat>(new_x), 0.0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
}

GLuint Sprite::loadShader(const char *shaderPath, GLuint type)
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

void Sprite::compileShaders(const char *FS, const char *VS)
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

    gObjectLocation = glGetUniformLocation(shader, "object");
    gCameraParamsLocation = glGetUniformLocation(shader, "cameraParams");
    gPersProjParamsLocation = glGetUniformLocation(shader, "PersProjParams");

    assert(gWorldLocation != 0xFFFFFFFF);
}

void Sprite::initializeGeometry()
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

    geometryInfo.numVertices = vertices.size();
    geometryInfo.numIndices = indices.size();

    glGenVertexArrays(1, &geometryInfo.VAO);
    glBindVertexArray(geometryInfo.VAO);

    glGenBuffers(1, &geometryInfo.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, geometryInfo.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &geometryInfo.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometryInfo.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

struct GeometryInfo *Sprite::GetGeometry()
{
    return &geometryInfo;
}


Sprite::Sprite(const std::string &_name, const char *FS, const char *VS, const char *texturePath)
    : name(_name)
{
    compileShaders(FS, VS);
    loadTextures(texturePath);
}

Sprite::Sprite() {}
