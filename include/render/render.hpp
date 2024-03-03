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
    void drawObject(Sprite &_sprite);
    // void drawObject(sphere &_sprite);
    // void drawObject(circle &_sprite);
    // void drawObject(line &_sprite);
    // void drawObject(cube_bone &_sprite);

    Pipeline pipeline;
};
