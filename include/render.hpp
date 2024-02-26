#pragma once
#include "sprite.hpp"
#include "pipeline.hpp"

class Render
{
public:
    Render(Pipeline &_pipeline);
    Render(Camera *_camera);
    
    void clearRender();
    void drawObeject(sprite &_sprite);

    Pipeline pipeline;
};
