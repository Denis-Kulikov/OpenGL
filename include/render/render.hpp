#pragma once
#include "pipeline.hpp"
#include "../object/sprite.hpp"

class Render
{
public:
    Render(Pipeline &_pipeline);
    Render(Camera *_camera);
    
    void SetCamera(Camera *_camera);
    void clearRender();
    void PushGeometry(struct GeometryInfo *geometry);
    void drawObject(objectTransform *transform, Sprite *sprite) const;

    Pipeline pipeline;
};
