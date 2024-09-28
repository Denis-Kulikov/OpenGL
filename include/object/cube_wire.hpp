#pragma once
#include "line.hpp"

class Cube_wire : public Line
{
public:
    Cube_wire(const std::string &_name, const glm::vec3 _color);
    Cube_wire();
    static void initializeGeometry();
    struct GeometryInfo *GetGeometry() override;
    void Render(void *RenderData) const override;

private:
    static struct GeometryInfo geometryInfo;
};
