#pragma once
#include "glfw.hpp"

class Render
{
public:
    Render(const Pipeline &_pipeline);
    
    void clearRender();
    void drawObeject();

    Pipeline pipeline;
};
