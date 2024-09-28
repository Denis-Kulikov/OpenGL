#pragma once
#include "sprite.hpp"

class Line : public Sprite
{
public:
    Line(const std::string &_name, const glm::vec3 _color);
    Line();

    virtual void Render(void *RenderData) const override;
    objectTransform setPoints(const glm::vec3 &_start, const glm::vec3 &_end);
    static void initializeGeometry();
    struct GeometryInfo *GetGeometry() override;
    void SetColor(glm::vec3 _color) {
        color = _color;
    }
    void SetWidth(GLfloat _width) {
        width = _width;
    }

protected:
    GLuint gColorLocation;
    GLfloat width = 2.0f;
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    static struct GeometryInfo geometryInfo;
};
