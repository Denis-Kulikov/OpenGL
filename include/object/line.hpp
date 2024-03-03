#pragma once
#include "sprite.hpp"

class line : public Sprite
{
public:
    // line(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> _color, GLfloat _lineWidth);
    line(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> _color);

    void setPoints(const Vector3<GLfloat> &_start, const Vector3<GLfloat> &_end);
    static void initializeGeometry();

    GLfloat lineWidth;

private:
    static struct GeometryInfo geometryInfo;
};
