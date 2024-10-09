#include <render/render.hpp>

#include <chrono>
extern std::chrono::milliseconds totalTime;

Render::Render(const Pipeline &_pipeline)
{
    pipeline.camera = _pipeline.camera;
}

Render::Render(const Camera &_camera)
{
    pipeline.camera = _camera;
}

Render::Render()
{}

void Render::SetCamera(const Camera &_camera)
{
    pipeline.camera = _camera;
}

void Render::clearRender() {
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(2);

    shader = 0;
    texture = 0;
}

void Render::PushLineWidth(GLfloat _LineWidth)
{
    if (LineWidth != _LineWidth) {
        LineWidth = _LineWidth;
        glLineWidth(_LineWidth);
    }
}

void Render::PushGeometry(const struct GeometryInfo *geometry)
{
    glBindVertexArray(geometry->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->EBO);
}

void Render::drawObject(Matrix4f& matrix, Sprite *sprite)
{
    if (sprite == nullptr) {
        std::cout << "Error Render.drawObject(): Sprite == nullptr" << std::endl;
        return;
    }

    if (shader != sprite->shader) {
        glUseProgram(sprite->shader);
        shader = sprite->shader;
    }

    if (texture != sprite->texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sprite->texture);
        texture = sprite->texture;
        glUniform1i(sprite->gTextureSamplerLocation, 0);
    }

    glUniformMatrix4fv(sprite->gWorldLocation, 1, GL_TRUE, &matrix);

    if (sprite->GetGeometry()->EBO != 0) {
        glDrawElements(GL_TRIANGLES, sprite->GetGeometry()->numIndices, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_LINE_STRIP, 0, sprite->GetGeometry()->numVertices);
    }
}

void Render::drawSkybox(Cube &skybox)
{
    GLint OldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
    GLint OldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);
 
    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    PushGeometry(skybox.GetGeometry());

    objectTransform skybox_transform;
    skybox_transform.SetWorldPos(pipeline.camera.GetPosition());
    skybox_transform.SetRotate(glm::vec3(0.0, 0.0, 180));
    skybox_transform.SetScale(glm::vec3(2, 2, 2));
    
    glDepthMask(GL_FALSE);
    auto mat4x4 = pipeline.GetTransform(skybox_transform);
    drawObject(mat4x4, &skybox);
    glDepthMask(GL_TRUE);

    glCullFace(OldCullFaceMode);
    glDepthFunc(OldDepthFuncMode);
}

void Render::GetPV() {
    Matrix4f CameraTranslationTrans, CameraRotateTrans;
    CameraTranslationTrans.InitTranslationTransform(-pipeline.camera.GetPosition().x, -pipeline.camera.GetPosition().y, -pipeline.camera.GetPosition().z);
    CameraRotateTrans.InitCameraTransform(pipeline.camera.Params.Target, pipeline.camera.Params.Up);
    View = CameraRotateTrans * CameraTranslationTrans;
    PersProjTrans.InitPersProjTransform(pipeline.camera.PersProj.FOV, pipeline.camera.PersProj.Width, pipeline.camera.PersProj.Height, pipeline.camera.PersProj.zNear, pipeline.camera.PersProj.zFar);
    PV = PersProjTrans * View;
}
