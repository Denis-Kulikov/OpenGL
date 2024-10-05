#pragma once
#include "pipeline.hpp"
#include "../object/sprite.hpp"
#include "../object/cube.hpp"

class Render
{
public:
    Render(const Pipeline &_pipeline);
    Render(const Camera &_camera);
    Render();
    
    void SetCamera(const Camera &_camera);
    void clearRender();
    void PushLineWidth(GLfloat _LineWidth);
    void PushGeometry(const struct GeometryInfo *geometry);
    void drawObject(Matrix4f& matrix, Sprite *sprite);
    void drawSkybox(Cube &skybox);
    void GetPV();

    Pipeline pipeline;

    GLuint shader = 0;
    GLuint texture = 0;

    GLfloat LineWidth = -1;

    Matrix4f PersProjTrans;
    Matrix4f View;
    Matrix4f PV;
};
