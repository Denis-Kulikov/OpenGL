#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <assimp/scene.h>
#include <iostream>

namespace tinygltf {
    class Image;
}

class Texture {
public:
    Texture(const std::string& path);
    Texture(const aiTexture* texture);
    Texture(const tinygltf::Image& texture);

    void Bind() const;
    GLuint GetID() const;
    float GetScale() const;

    static Texture* Create(const std::string& name, const std::string& path);
    static Texture* Create(const std::string& name, const aiTexture* texture);
    static Texture* Create(const std::string& name, const tinygltf::Image& texture);
    static Texture* Find(const std::string& name);
    static void Delete(const std::string& path);
    static void ClearСache();

private:
    void Load(std::string path);
    void Load(const aiTexture* texture);
    void Load(const tinygltf::Image& texture);

    GLuint textureID;
    float scale;
    inline static std::unordered_map<std::string, Texture> cache;
};
