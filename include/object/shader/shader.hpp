#pragma once
#include "lib-project/lib.hpp"
#include <stb_image.h>

class Shader {
public:
    Shader(const std::string &FS, const std::string &VS);
    Shader(const std::size_t id);
    Shader();

private:
    class ShaderID {
    public:
        ShaderID(const std::size_t id);
        ShaderID();
        ~ShaderID();
        operator GLuint() const { return shaderID; }
        operator Shader() const { return shaderID; }

        inline static std::map<std::string, ShaderID> shadersMap;
    private:
        GLuint shaderID;
    };

    ShaderID operator *() const { return shaderProgramm; }
    GLuint loadShader(const std::string &shaderPath, GLuint type);
    GLuint shaderProgramm;

public:
    static Shader shader_find(const std::string &name);
    static void shader_push(const std::string &name, Shader shaderProgramm);
    operator GLuint() const { return shaderProgramm; }
};
