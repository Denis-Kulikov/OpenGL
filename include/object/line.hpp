#pragma once
#include "sprite.hpp"

class line : public Sprite
{
public:
    line(const std::string &_name, const objectTransform &_trans, const glm::vec3 _color);
    line(const std::string &_name, const glm::vec3 _color);

    virtual void Render(void *RenderData) const override;
    void setPoints(const glm::vec3 &_start, const glm::vec3 &_end);
    static void initializeGeometry();
    struct GeometryInfo *GetGeometry() override;

    GLfloat lineWidth;
    objectTransform trans;
private:
    static struct GeometryInfo geometryInfo;
};
