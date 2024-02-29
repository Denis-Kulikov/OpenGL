#pragma once
#include "pipeline.hpp"
#include "../object/sprite.hpp"
#include "../object/sphere.hpp"
#include "../object/circle.hpp"
#include "../object/line.hpp"
#include "../object/cube_bone.hpp"

class Render
{
public:
    Render(Pipeline &_pipeline);
    Render(Camera *_camera);
    
    void clearRender();
    void drawObeject(sprite &_sprite);
    // void drawObeject(sphere &_sprite);
    // void drawObeject(circle &_sprite);
    // void drawObeject(line &_sprite);
    // void drawObeject(cube_bone &_sprite);

    Pipeline pipeline;
};
