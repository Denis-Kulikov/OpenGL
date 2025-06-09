#include <iostream>
#include <fstream>
#include <object/material/shader.hpp>
#include <sstream>

Shader::Shader(const std::string& name_, const std::string& FS, const std::string& VS)
{
    name = name_;
    auto it = Find(name);
    if (it != nullptr) {
        *this = *it;
    } else {
        // std::cout << "new shader:" << FS+VS << std::endl;
        Link(FS, VS);
    }
}

Shader::Shader(const std::string &FS, const std::string &VS)
    : Shader(FS + VS, FS, VS)
{}

Shader* Shader::Create(const std::string& name, const std::string& FS, const std::string& VS) {
    auto [it, inserted] = cache.try_emplace(name, FS, VS);
    return &it->second;
}


void Shader::Link(const std::string &FS, const std::string &VS) {
    shaderProgram = glCreateProgram();

    GLuint fragmentShader;
    GLuint vertexShader;
    fragmentShader = Compile(FS, GL_FRAGMENT_SHADER);
    vertexShader = Compile(VS, GL_VERTEX_SHADER);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, vertexShader);
    glLinkProgram(shaderProgram);

    GLint ok;
    GLchar log[2000];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint infoLogLength;
        GLchar* infoLog;
        glGetProgramInfoLog(shaderProgram, 2000, NULL, log);
        std::cout << "Shader (" << FS << ", " << VS << ") compilation Log:\n" << log << std::endl;
    
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, infoLog);
        std::cout << "Shader fragmentShader Log:\n" << infoLog << std::endl;
        delete[] infoLog;

        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(vertexShader, infoLogLength, NULL, infoLog);
        std::cout << "Shader vertexShader Log:\n" << infoLog << std::endl;
        delete[] infoLog;

        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        
        std::cout << std::endl;
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "Shader::Link OpenGL error: " << error << std::endl;
    }
}

GLuint Shader::Compile(const std::string &ShaderPath, GLuint type)
{
    std::ifstream ShaderFile(ShaderPath);
    if (!ShaderFile.is_open()) {
        std::cerr << "Error: Could not open Shader file '" << ShaderPath << "'" << std::endl;
        return 0;
    }

    std::stringstream ShaderStream;
    ShaderStream << ShaderFile.rdbuf();
    ShaderFile.close();

    std::string ShaderCode = ShaderStream.str();
    const GLchar* ShaderCodePtr = ShaderCode.c_str();

    GLuint Shader = glCreateShader(type);
    glShaderSource(Shader, 1, &ShaderCodePtr, NULL);
    glCompileShader(Shader);

    GLint ok;
    GLchar log[2000];
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(Shader, 2000, NULL, log);
        printf("Shader(%s): %s\n", ShaderPath.c_str(), log);
        std::cout << ShaderCode << std::endl;
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "Shader::Compile OpenGL error: " << error << std::endl;
    }

    return Shader;
}

GLuint Shader::GetID() const {
    return shaderProgram;
}

void Shader::Delete(const std::string &name) {
    auto it = cache.find(name);
    if (it != cache.end()) {
        GLuint id = it->second.GetID();
        if (glIsProgram(it->second.GetID())) 
            glDeleteTextures(1, &id);
        cache.erase(it); 
    }
}

Shader* Shader::Find(const std::string &name) {
    auto it = cache.find(name);
    return it != cache.end() ? &it->second : nullptr;
}
