#pragma once
#include "../object/shader/shader.hpp"

class RenderableObject {
public:
    RenderableObject() {}
    virtual void Render(void *RenderData) const = 0;
};
