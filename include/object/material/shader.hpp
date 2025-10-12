#pragma once
#include <GL/glew.h>
#include <string>
#include <stb_image.h>
#include <unordered_map>
#include <memory>

struct UniformInfo {
    GLint location = 0;
    GLint size = 0;
    GLenum type = 0;
};

struct AttributeInfo : public UniformInfo {
    GLint attribute = 0;
};

class Shader {
public:
    Shader(const std::string& FS, const std::string& VS);
    void Bind() const;
    GLuint GetID() const;
    const UniformInfo* FindUniform(const std::string& name) const;

    static Shader* Create(const std::string& name, const std::string& FS, const std::string& VS);
    static Shader* Find(const std::string &name);
    static void Delete(const std::string &name);
    static void ClearСache();
    
    std::unordered_map<std::string, UniformInfo> uniforms;
    std::unordered_map<std::string, AttributeInfo> attributes;

private:
    GLuint Compile(const std::string& ShaderPath, GLuint type);
    void Link(const std::string &FS, const std::string &VS);
    void RegisterAttributes();
    void RegisterUniforms();
    void RegisterUBOs();

    GLuint id = 0;
    inline static std::unordered_map<std::string, Shader> cache;
};
