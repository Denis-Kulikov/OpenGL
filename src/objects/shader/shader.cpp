#include <object/shader/shader.hpp>


Shader::ShaderID::ShaderID(const std::size_t id)
    : shaderID(id)
{}
Shader::ShaderID::ShaderID()
    : ShaderID(0)
{}


Shader::ShaderID::~ShaderID() {
    if (shaderID) 
        glDeleteProgram(shaderID);
}


Shader::Shader(const std::size_t id)
    : shaderProgramm(id)
{}

Shader::Shader()
    : Shader(0)
{}

Shader::Shader(const std::string &FS, const std::string &VS)
{
    auto shader = Shader::shader_find(std::string(FS) + std::string(VS));
    if (shader != 0) {
        shaderProgramm = shader;
        return;
    }

    shaderProgramm = glCreateProgram();

    GLuint fragmentShader;
    GLuint vertexShader;
    fragmentShader = loadShader(FS, GL_FRAGMENT_SHADER);
    vertexShader = loadShader(VS, GL_VERTEX_SHADER);
    glAttachShader(shaderProgramm, fragmentShader);
    glAttachShader(shaderProgramm, vertexShader);
    glLinkProgram(shaderProgramm);

    GLint ok;
    GLchar log[2000];
    glGetProgramiv(shaderProgramm, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint infoLogLength;
        GLchar* infoLog;
        glGetProgramInfoLog(shaderProgramm, 2000, NULL, log);
        std::cout << "Shader " << " compilation Log:\n" << log << std::endl;
    
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
}

GLuint Shader::loadShader(const std::string &shaderPath, GLuint type)
{
    std::ifstream shaderFile(shaderPath);
    if (!shaderFile.is_open()) {
        std::cerr << "Error: Could not open shader file '" << shaderPath << "'" << std::endl;
        return 0;
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    std::string shaderCode = shaderStream.str();
    const GLchar* shaderCodePtr = shaderCode.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCodePtr, NULL);
    glCompileShader(shader);

    GLint ok;
    GLchar log[2000];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(shader, 2000, NULL, log);
        printf("Shader(%s): %s\n", shaderPath, log);
        std::cout << shaderCode << std::endl;
    }

    return shader;
}

Shader Shader::shader_find(const std::string &name)
{
    auto programm = ShaderID::shadersMap.find(name);
    return programm == ShaderID::shadersMap.end() ? 0 : programm->second;
}

void Shader::shader_push(const std::string &name, Shader shaderProgramm)
{
    auto programm = ShaderID::shadersMap.find(name);
    if (programm != ShaderID::shadersMap.end())
        glDeleteProgram(programm->second);

    ShaderID::shadersMap[name] = *shaderProgramm;
}
