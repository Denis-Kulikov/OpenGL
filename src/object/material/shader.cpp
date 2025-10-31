#include <iostream>
#include <fstream>
#include <object/material/shader.hpp>
#include <sstream>
#include <managers/render/render.hpp>

Shader::Shader(const std::string& FS, const std::string& VS)
{
    Link(FS, VS);
    RegisterAttributes();
    RegisterUniforms();
    RegisterUBOs();
}

void Shader::Link(const std::string &FS, const std::string &VS) {
    id = glCreateProgram();

    GLuint fragmentShader;
    GLuint vertexShader;
    fragmentShader = Compile(FS, GL_FRAGMENT_SHADER);
    vertexShader = Compile(VS, GL_VERTEX_SHADER);
    glAttachShader(id, fragmentShader);
    glAttachShader(id, vertexShader);
    glLinkProgram(id);

    GLint ok;
    GLchar log[2000];
    glGetProgramiv(id, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint infoLogLength;
        GLchar* infoLog;
        glGetProgramInfoLog(id, 2000, NULL, log);
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

void Shader::RegisterAttributes() {
    GLint numAttribs = 0;
    GLint maxNameLength = 0;
    GLsizei length = 0;
    GLint size = 0;
    GLenum type = 0;
    GLchar name[256];

    glGetProgramiv(GetID(), GL_ACTIVE_ATTRIBUTES, &numAttribs);
    glGetProgramiv(GetID(), GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);

    for (int i = 0; i < numAttribs; ++i) {
        glGetActiveAttrib(GetID(), i, maxNameLength, &length, &size, &type, &name[0]);
        std::string nameAttribute(&name[0], length);
        GLint loc = glGetAttribLocation(GetID(), nameAttribute.c_str());
        attributes[name] = {loc, size, type, i};
    }
}

void Shader::RegisterUniforms() {
    GLint count;
    GLint size;
    GLenum type;
    GLchar name[256];

    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);
    uniforms.reserve(count);
    for (int i = 0; i < count; i++) {
        glGetActiveUniform(id, i, sizeof(name), nullptr, &size, &type, name);
        std::string uniformName = name;
        if (uniformName.size() > 3) {
            if (uniformName.substr(uniformName.size() - 3) == "[0]") {
                uniformName = uniformName.substr(0, uniformName.size() - 3);
            }
        }
        std::cout << "Uniform: " << uniformName << " size: " << size << std::endl;
        GLint loc = glGetUniformLocation(id, uniformName.c_str());
        uniforms[uniformName] = {loc, size, type};
    }
}

void Shader::RegisterUBOs() {
    const auto& buffers = RenderManager::bufferManager.buffers;
    GLint numUniformBlocks, numStorageBlocks;
    glGetProgramiv(id, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);
    glGetProgramInterfaceiv(id, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &numStorageBlocks);
     
    // UBO
    for (int i = 0; i < numUniformBlocks; ++i) {
        char name[256];
        glGetActiveUniformBlockName(id, i, sizeof(name), nullptr, name);
        auto it = buffers.find(name);
        if (it != buffers.end() && it->second.type == BufferType::Uniform)
            glUniformBlockBinding(id, i, (GLuint)it->second.binding);
    }

    // SSBO
    for (int i = 0; i < numStorageBlocks; ++i) {
        char name[256];
        glGetProgramResourceName(id, GL_SHADER_STORAGE_BLOCK, i, sizeof(name), nullptr, name);
        auto it = RenderManager::bufferManager.buffers.find(name);
        if (it != buffers.end() && it->second.type == BufferType::Storage) {
            GLuint blockIndex = glGetProgramResourceIndex(id, GL_SHADER_STORAGE_BLOCK, name);
            glShaderStorageBlockBinding(id, blockIndex, (GLuint)it->second.binding);
        }
    }
}

void Shader::Bind() const {
    glUseProgram(GetID());
}

GLuint Shader::GetID() const {
    return id;
}

const UniformInfo* Shader::FindUniform(const std::string& name) const {
    auto it = uniforms.find(name);
    return it == uniforms.end() ? nullptr : &it->second;
}

Shader* Shader::Create(const std::string& name, const std::string& FS, const std::string& VS) {
    auto [it, inserted] = cache.try_emplace(name, FS, VS);
    return &it->second;
}
Shader* Shader::Find(const std::string &name) {
    auto it = cache.find(name);
    return it != cache.end() ? &it->second : nullptr;
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
void Shader::ClearСache() {
    for (auto it = cache.begin(); it != cache.end(); ) {
        if (glIsTexture(it->second.GetID())) {
            GLuint id = it->second.GetID();
            glDeleteShader(id);
        }
        it = cache.erase(it);
    }
}