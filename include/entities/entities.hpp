#pragma once
#include "objectTransform.hpp"

class sprite 
{
public:
    sprite(const std::string &_name, const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath);
    sprite(const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath);

    std::string      name;
    objectTransform  trans;
    
protected:
    GLuint loadShader(const char *shaderPath, GLuint type);
    void loadTexures(const char *texturePath);
    void compileShaders(const char *FS, const char *VS);

private:
    void initializeGeometry();

};
