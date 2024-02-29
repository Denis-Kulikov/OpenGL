#pragma once
#include "line.hpp"

class cube_bone : public line
{
public:
    cube_bone(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> _color);
    cube_bone(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> _color, GLfloat _lineWidth);
    static void initializeGeometry();

private:
    static struct GeometryInfo geometryInfo;
};
