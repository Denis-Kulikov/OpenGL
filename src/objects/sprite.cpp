#include <render/glfw.hpp>
#include <object/sprite.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_ERRORS_H


GLuint createTextTexture(FT_Library ft, const char* fontPath, const char* text, int fontSize) {
    auto txr = Sprite::GetTexture(std::string(text));
    if (txr) return txr;

    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face)) {
        std::cerr << "Failed to load font" << std::endl;
        return 0;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    int width = 0;
    int height = 0;

    // Calculate the size of the texture
    for (const char* p = text; *p; p++) {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load Glyph for character: " << *p << std::endl;
            continue;
        }

        width += (face->glyph->advance.x >> 6); // advance.x is in 1/64th pixels, shift by 6 to get pixels
        height = std::max(height, static_cast<int>(face->glyph->bitmap.rows));
    }

    // Create a 2D buffer for the texture
    std::vector<std::vector<unsigned char>> buffer(height, std::vector<unsigned char>(width, 0));

    int xOffset = 0;
    for (const char* p = text; *p; p++) {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load Glyph for character: " << *p << std::endl;
            continue;
        }

        // Copy glyph bitmap into buffer
        for (int row = 0; row < face->glyph->bitmap.rows; ++row) {
            for (int col = 0; col < face->glyph->bitmap.width; ++col) {
                buffer[row][xOffset + col] = face->glyph->bitmap.buffer[row * face->glyph->bitmap.width + col];
            }
        }

        xOffset += (face->glyph->advance.x >> 6); // Move to the next character position
    }

    // Convert 2D buffer to 1D array for OpenGL
    std::vector<unsigned char> buffer1D;
    for (const auto& row : buffer) {
        buffer1D.insert(buffer1D.end(), row.begin(), row.end());
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer1D.data());

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    Sprite::PushTexture(texture, std::string(text));

    FT_Done_Face(face);

    return texture;
}





Sprite::Sprite(const std::string &_name, const char *FS, const char *VS, const char *texturePath)
    : name(_name)
{
    auto sdr = shadersMap.find(std::string(FS) + std::string(VS));
    if (sdr != shadersMap.end()) {
        shader                  = sdr->second[0];
        gWorldLocation          = sdr->second[1];
        gColorLocation          = sdr->second[2];
        gTextureSamplerLocation = sdr->second[3];
    } else {
        compileShaders(FS, VS);
        shadersMap[std::string(FS) + std::string(VS)] = std::array<GLuint, 4>{shader, gWorldLocation, gColorLocation, gTextureSamplerLocation};
    }

    auto txr = texturesMap.find(std::string(texturePath));
    if (txr != texturesMap.end()) {
        texture = txr->second;
    } else {
        loadTextures(texturePath);
        texturesMap[std::string(texturePath)] = texture;
    }
}

Sprite::Sprite(const std::string &_name, const char *FS, const char *VS, const char *texturePath, const Vector3<GLfloat> &_color)
    : Sprite(_name, FS, VS, texturePath)
{
    color = _color;
}


struct GeometryInfo *Sprite::GetGeometry()
{
    return &geometryInfo;
}

void Sprite::PushTexture(const GLuint new_texture, const std::string &name)
{
    texturesMap[std::string(name)] = new_texture;
}

void Sprite::SetTexture(const std::string &name)
{
    auto txr = texturesMap.find(std::string(name));
    if (txr != texturesMap.end()) {
        texture = txr->second;
    } else {
        std::cout << "Texture '" << name << "' was not found" << std::endl;
    }
}

GLuint Sprite::GetTexture(const std::string &name)
{
    auto txr = texturesMap.find(std::string(name));
    return txr != texturesMap.end() ? txr->second : 0;
}


void Sprite::compileShaders(const char *FS, const char *VS)
{
    if (shader == 0) 
        shader = glCreateProgram();

    GLuint fragmentShader = 0;
    GLuint vertexShader = 0;

    if (FS != nullptr) {
        fragmentShader = loadShader(FS, GL_FRAGMENT_SHADER);
        if (fragmentShader == 0) {
            std::cerr << "Fragment Shader compilation error" << std::endl;
            return;
        }
        glAttachShader(shader, fragmentShader);
    }
    if (VS != nullptr) {
        vertexShader = loadShader(VS, GL_VERTEX_SHADER);
        if (vertexShader == 0) {
            std::cerr << "Vertex Shader compilation error" << std::endl;
            return;
        }
        glAttachShader(shader, vertexShader);
    }

    glLinkProgram(shader);

    GLint ok;
    GLchar log[2000];
    glGetProgramiv(shader, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(shader, sizeof(log), NULL, log);
        std::cerr << "Shader " << name << " compilation Log:\n" << log << std::endl;
    
        if (fragmentShader != 0) {
            GLint infoLogLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0) {
                GLchar *infoLog = new GLchar[infoLogLength];
                glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, infoLog);
                std::cerr << "Shader fragmentShader Log:\n" << infoLog << std::endl;
                delete[] infoLog;
            }
        }

        if (vertexShader != 0) {
            GLint infoLogLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0) {
                GLchar *infoLog = new GLchar[infoLogLength];
                glGetShaderInfoLog(vertexShader, infoLogLength, NULL, infoLog);
                std::cerr << "Shader vertexShader Log:\n" << infoLog << std::endl;
                delete[] infoLog;
            }
        }
        return;
    }

    gWorldLocation = glGetUniformLocation(shader, "gWorld");
    gColorLocation = glGetUniformLocation(shader, "gColor");
    gTextureSamplerLocation = glGetUniformLocation(shader, "textureSampler");

    assert(gWorldLocation != 0xFFFFFFFF);
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
        // std::cout << "Shader(" << shaderPath << "): " << log << std::endl; 
        std::cout << shaderCode << std::endl;
    }

    return shader;
}

void Sprite::loadTextures(const char *texturePath) {
    if (texturePath == nullptr) return;

    int x, y, n;
    std::string path = std::string("assets/") + texturePath;
    unsigned char *img = stbi_load(path.c_str(), &x, &y, &n, 0);

    if (img == nullptr) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return;
    }

    // Determine new dimensions for texture to be power of two
    int new_x = 1 << (int)std::ceil(std::log2(x));
    int new_y = 1 << (int)std::ceil(std::log2(y));

    unsigned char *resized_img = (unsigned char*)malloc(new_x * new_y * n);
    if (resized_img == nullptr) {
        std::cerr << "Failed to allocate memory for resized texture" << std::endl;
        stbi_image_free(img);
        return;
    }

    // Resize image to the new dimensions
    stbir_resize_uint8(img, x, y, 0, resized_img, new_x, new_y, 0, n);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLenum format = (n == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, new_x, new_y, 0, format, GL_UNSIGNED_BYTE, resized_img);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(img);
    free(resized_img);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
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

    glBindVertexArray(0);
}
