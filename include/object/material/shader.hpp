#pragma once
#include <GL/glew.h>
#include <string>
#include <stb_image.h>
#include <unordered_map>
#include <memory>

struct UniformInfo {
    GLint location;
    GLenum type;
    GLint size;
};


class Shader {
public:
    Shader(const std::string& FS, const std::string& VS);
    void Bind() const;
    GLuint GetID() const;
    const UniformInfo* FindUniform(const std::string& name) const {
        auto it = uniforms.find(name);
        return it == uniforms.end() ? nullptr : &it->second;
    }

    static Shader* Create(const std::string& name, const std::string& FS, const std::string& VS);
    static Shader* Find(const std::string &name);
    static void Delete(const std::string &name);
    static void ClearСache();
    
    std::unordered_map<std::string, UniformInfo> uniforms;

private:
    GLuint Compile(const std::string& ShaderPath, GLuint type);
    void Link(const std::string &FS, const std::string &VS);
    void RegisterUniforms();

    GLuint id;
    inline static std::unordered_map<std::string, Shader> cache;
};
