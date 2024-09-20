#pragma once
#include "pipeline.hpp"
#include "../object/sprite.hpp"
#include "../object/cube.hpp"

class Render
{
public:
    Render(Pipeline &_pipeline);
    Render(Camera *_camera);
    Render();
    
    void SetCamera(Camera *_camera);
    void clearRender();
    void PushGeometry(struct GeometryInfo *geometry);
    void drawObject(Matrix4f& matrix, Sprite *sprite);
    void drawSkybox(Cube &skybox);

    Pipeline pipeline;

    GLuint shader = 0;
    GLuint texture = 0;
};
