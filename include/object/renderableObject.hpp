#pragma once
#include "../object/shader/shader.hpp"

class RenderableObject {
public:
    virtual void Render(void *RenderData) const = 0;
};
