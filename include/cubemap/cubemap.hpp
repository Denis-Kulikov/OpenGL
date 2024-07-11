#pragma once
#include "lib-project/lib.hpp"

class Cubemap 
{
public:
    Cubemap(const std::string& Directory,
            const std::string& PosXFilename, const std::string& NegXFilename,
            const std::string& PosYFilename, const std::string& NegYFilename,
            const std::string& PosZFilename, const std::string& NegZFilename);
 
    ~Cubemap();
 
    bool Load();
    void Bind(GLenum TextureUnit);
 
private:
    GLuint texture;
};
