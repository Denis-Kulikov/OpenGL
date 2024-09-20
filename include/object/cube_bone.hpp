#pragma once
#include "line.hpp"

class cube_bone : public line
{
public:
    cube_bone(const std::string &_name, const objectTransform &_trans, const glm::vec3 _color);
    static void initializeGeometry();
    struct GeometryInfo *GetGeometry() override;

private:
    static struct GeometryInfo geometryInfo;
};
