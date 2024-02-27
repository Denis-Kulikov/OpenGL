#pragma once
#include "sprite.hpp"

class circle : public sprite
{
public:
    circle(const std::string &_name, const objectTransform &_trans, const char *FS, const char *VS, const char *texturePath, const GLint _numVertices);

private:
    void initializeGeometry();
};
