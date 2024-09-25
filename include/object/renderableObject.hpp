#pragma once

class RenderableObject {
public:
    RenderableObject() {}
    virtual void Render(void *RenderData) const = 0;
};
