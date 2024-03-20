#pragma once
#include "sprite.hpp"

class sphere : public Sprite
{
public:
    sphere(const std::string &_name, const char *FS, const char *VS, const char *texturePath, const GLint _numVertices);
    sphere() : Sprite() {};
    struct GeometryInfo *GetGeometry() override;

private:
    void initializeGeometry(const GLint _numVertices);
    struct GeometryInfo *geometry = nullptr; 
};
