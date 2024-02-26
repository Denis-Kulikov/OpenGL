#pragma once
#include "glfw.hpp"
#include "sprite.hpp"

class Render
{
public:
    Render(const Pipeline &_pipeline);
    
    void clearRender();
    void drawObeject(sprite &_sprite);

    Pipeline pipeline;
};
