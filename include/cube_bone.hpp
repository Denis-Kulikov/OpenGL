#pragma once
#include "glfw.hpp"
#include "sprite.hpp"

class cube_bone : public sprite
{
public:
    cube_bone(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> _color);

private:
    void initializeGeometry();
};
