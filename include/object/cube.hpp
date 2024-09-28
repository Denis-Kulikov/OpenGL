#pragma once
#include "sprite.hpp"

class Cube : public Sprite
{
public:
    Cube(const std::string &texture_path);
    Cube();
    static void initializeGeometry();
    struct GeometryInfo *GetGeometry() override;
    virtual void Render(void *RenderData) const override;

private:
    static struct GeometryInfo geometryInfo;
};
