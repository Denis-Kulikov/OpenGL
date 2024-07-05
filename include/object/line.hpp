#pragma once
#include "sprite.hpp"

class line : public Sprite
{
public:
    line(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> _color);
    line(const std::string &_name, const Vector3<GLfloat> _color);

    void setPoints(const Vector3<GLfloat> &_start, const Vector3<GLfloat> &_end);
    static void initializeGeometry();
    struct GeometryInfo *GetGeometry() override;

    GLfloat lineWidth;
    objectTransform trans;
private:
    static struct GeometryInfo geometryInfo;
};
