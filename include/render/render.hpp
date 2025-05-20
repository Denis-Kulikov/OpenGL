#pragma once
#include "pipeline.hpp"
#include "../object/sprite.hpp"
#include "../object/cube.hpp"
#include <math/Matrix4f.hpp>

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
    void drawObject(glm::mat4x4& matrix, Sprite *sprite);
    void drawSkybox(Cube &skybox);

    glm::mat4 GetPV();
    // Perspective - true, Orthographic - false
    void UpdatePV(bool perspective);
    void UpdatePV_Perspective();
    void UpdatePV_Orthographic();

    Pipeline pipeline;

    GLuint shader = 0;
    GLuint texture = 0;

    GLfloat LineWidth = -1;

    glm::mat4 ProjTrans;
    glm::mat4 View;
    glm::mat4 PV;
};
