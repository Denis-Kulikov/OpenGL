#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <assimp/scene.h>
#include <iostream>

class Texture {
public:
    Texture(const std::string& name, const aiTexture* texture);
    Texture(const std::string& name, const std::string& path);
    Texture(const std::string& path);

    static Texture* Create(const std::string& name, const std::string& path);
    static Texture* Create(const std::string& name, const aiTexture* texture);

    void Bind() const;
    GLuint GetID() const;
    float GetScale() const;

    static Texture* Find(const std::string& name);
    static void Delete(const std::string& path);
    static void ClearChashe();

private:
    void Load(std::string path);
    void Load(const aiTexture* texture);

    GLuint textureID;
    float scale;
    std::string name;
    inline static std::unordered_map<std::string, Texture> cache;
};
