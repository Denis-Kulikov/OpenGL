#pragma once
#include "sprite.hpp"

class Cube : public Sprite
{
public:
    Cube();
    static void initializeGeometry();
    struct GeometryInfo *GetGeometry() override;

private:
    static struct GeometryInfo geometryInfo;
};
