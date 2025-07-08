#pragma once
#include <GL/glew.h>
#include <string>
#include <stb_image.h>
#include <unordered_map>
#include <memory>

class Shader {
public:
    Shader(const std::string& FS, const std::string& VS);

    void Bind() const;
    GLuint GetID() const;
    static Shader* Create(const std::string& name, const std::string& FS, const std::string& VS);
    static Shader* Find(const std::string &name);
    static void Delete(const std::string &name);
    static void ClearСache();

private:
    GLuint Compile(const std::string& ShaderPath, GLuint type);
    void Link(const std::string &FS, const std::string &VS);

    GLuint shaderProgram;
    inline static std::unordered_map<std::string, Shader> cache;
};
