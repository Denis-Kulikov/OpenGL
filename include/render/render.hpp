#pragma once
#include "pipeline.hpp"
#include "../object/sprite.hpp"

class Render
{
public:
    Render(Pipeline &_pipeline);
    Render(Camera *_camera);
    
    void clearRender();
    void drawObject(objectTransform &_transform, Sprite&_sprite);

    Pipeline pipeline;
};
