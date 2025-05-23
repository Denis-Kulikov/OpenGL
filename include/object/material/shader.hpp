#pragma once
#include <GL/glew.h>
#include <string>
#include <stb_image.h>
#include <unordered_map>
#include <memory>

class Shader {
public:
    Shader(const std::string& name_, const std::string& FS, const std::string& VS);
    Shader(const std::string& FS, const std::string& VS);

    GLuint GetID() const;
    static void Delete(const std::string &name);
    static Shader* Find(const std::string &name);
    static Shader* Create(const std::string& name, const std::string& FS, const std::string& VS);

    // static void ShaderPush(const std::string &name, Shader ShaderProgramm);

private:
    GLuint Compile(const std::string& ShaderPath, GLuint type);
    void Link(const std::string &FS, const std::string &VS);

    std::string name;
    GLuint shaderProgram;
    inline static std::unordered_map<std::string, Shader> cache;
};
