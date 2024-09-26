#include <game/gameManager.hpp>
#include <object/sprite.hpp>

#include <stb_image.h>
#include <stb_image_resize.h>


Sprite::Sprite(const std::string &_name, const std::string &FS, const std::string &VS, const std::string &texturePath)
    : name(_name), shader(FS, VS)
{
    std::cout << shader << std::endl;
    gWorldLocation = glGetUniformLocation(shader, "gWorld");
    assert(gWorldLocation != 0xFFFFFFFF);

    gColorLocation = glGetUniformLocation(shader, "gColor");
    gTextureSamplerLocation = glGetUniformLocation(shader, "textureSampler");


    auto txr = texturesMap.find(std::string(texturePath));
    if (txr != texturesMap.end()) {
        texture = txr->second;
    } else {
        loadTextures(texturePath);
        texturesMap[std::string(texturePath)] = texture;
    }
}

void Sprite::Render(void *RenderData) const {
    if (GameManager::render->pipeline.camera == nullptr) {
        std::cout << "Error Render: not found camera" << std::endl;
        return;
    }

    GameManager::render->PushGeometry(&geometryInfo);

    glUseProgram(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(gTextureSamplerLocation, 0);

    if (gColorLocation != 0xFFFFFFFF) glUniform3f(gColorLocation, color.x, color.y, color.z);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &static_cast<Sprite_rdata*>(RenderData)->matrix);

    glDrawElements(GL_TRIANGLES, geometryInfo.numIndices, GL_UNSIGNED_INT, 0);
}


struct GeometryInfo *Sprite::GetGeometry()
{
    return &geometryInfo;
}

void Sprite::loadTextures(const std::string &texturePath)
{
    int x, y, n;
    std::string path = std::string("assets/") + texturePath;
    unsigned char *img = stbi_load(path.c_str(), &x, &y, &n, 0);
    assert(img != nullptr);

    int new_x = 1 << (int)std::ceil(std::log2(x));
    int new_y = 1 << (int)std::ceil(std::log2(y));

    unsigned char*resized_img = (unsigned char*)malloc(new_x * new_y * n);
    if (resized_img == nullptr) {
        std::cerr << "Failed to allocate memory for resized texture" << std::endl;
        stbi_image_free(img);
        return;
    }

    stbir_resize_uint8(img, x, y, 0, resized_img, new_x, new_y, 0, n);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLenum format = (n == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, new_x, new_y, 0, format, GL_UNSIGNED_BYTE, resized_img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(img);
    free(resized_img);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLfloat scale_x = static_cast<GLfloat>(x) / static_cast<GLfloat>(new_x);
    GLfloat scale_y = static_cast<GLfloat>(y) / static_cast<GLfloat>(new_y);
    Scale = glm::vec3(1, static_cast<GLfloat>(new_y * scale_y) / static_cast<GLfloat>(new_x * scale_x), 0.0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
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
