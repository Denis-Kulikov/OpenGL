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
    void drawObject(Matrix4f<GLfloat>& matrix, Sprite *sprite);

    Pipeline pipeline;

    GLuint shader;
    GLuint texture;
};
