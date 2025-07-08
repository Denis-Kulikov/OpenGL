#include <object/material/texture.hpp>

#include <iostream>
#include <cmath>
#include <cassert>

#include <stb_image.h>
#include <stb_image_resize.h>

Texture::Texture(const aiTexture* texture) 
{
    Load(texture);
}

Texture::Texture(const std::string& path)
{
    Load(path);
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

GLuint Texture::GetID() const {
    return textureID;
}

float Texture::GetScale() const {
    return scale;
}

Texture* Texture::Create(const std::string& name, const std::string& path) {
    auto [it, inserted] = cache.try_emplace(name, path);
    return &it->second;
}

Texture* Texture::Create(const std::string& name, const aiTexture* texture) {
    auto [it, inserted] = cache.try_emplace(name, texture);
    return &it->second;
}
Texture* Texture::Find(const std::string& name) {
    auto it = cache.find(name);
    return it != cache.end() ? &it->second : nullptr;
}

void Texture::Delete(const std::string& path) {
    auto it = cache.find(path);
    if (it != cache.end()) {
        if (glIsTexture(it->second.GetID())) {
            GLuint id = it->second.GetID();
            glDeleteTextures(1, &id);

        }
        cache.erase(it); 
    }
}

void Texture::ClearСache() {
    for (auto it = cache.begin(); it != cache.end(); ) {
        if (glIsTexture(it->second.GetID())) {
            GLuint id = it->second.GetID();
            glDeleteTextures(1, &id);
        }
        it = cache.erase(it);
    }
}

void Texture::Load(std::string path) {
    int x, y, n;
    unsigned char *img = stbi_load(path.c_str(), &x, &y, &n, 0);
    std::cout << path << std::endl;
    assert(img != nullptr);

    int new_x = 1 << (int)std::ceil(std::log2(x));
    int new_y = 1 << (int)std::ceil(std::log2(y));

    unsigned char *resized_img = (unsigned char*)malloc(new_x * new_y * n);
    if (resized_img == nullptr) {
        std::cerr << "Failed to allocate memory for resized texture" << std::endl;
        stbi_image_free(img);
        return;
    }

    stbir_resize_uint8(img, x, y, 0, resized_img, new_x, new_y, 0, n);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

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

    scale = static_cast<GLfloat>(new_x) / static_cast<GLfloat>(new_y);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "Texture::Load OpenGL error: " << error << std::endl;
    }
}

void Texture::Load(const aiTexture* texture) {
    int width, height, channels;
    
    unsigned char* data = stbi_load_from_memory(
        reinterpret_cast<const unsigned char*>(texture->pcData), 
        texture->mWidth, 
        &width, &height, &channels, 0
    );

    
    scale = height > 0? static_cast<GLfloat>(width) / static_cast<GLfloat>(height) : 0;


    if (data == nullptr) {
        std::cerr << "Failed to load PNG from memory: " << stbi_failure_reason() << std::endl;
        textureID = 0;
        return;
    }

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}
