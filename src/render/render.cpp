#include <render/render.hpp>
#include <glm/gtc/matrix_transform.hpp>

Render::Render(const Pipeline &_pipeline)
{
    pipeline = _pipeline;
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

void Render::drawObject(glm::mat4x4& matrix, Sprite *sprite)
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

    glUniformMatrix4fv(sprite->gWorldLocation, 1, GL_FALSE, &matrix[0][0]);

    glDrawElements(GL_TRIANGLES, sprite->GetGeometry()->numIndices, GL_UNSIGNED_INT, 0);
}

void Render::drawSkybox(Cube &skybox)
{
    clearRender();

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
    skybox_transform.SetScale(glm::vec3(2));
    
    glDepthMask(GL_FALSE);
    auto mat4x4 = GetPV() * pipeline.GetModel(skybox_transform);
    skybox.Render(&mat4x4);
    // drawObject(mat4x4, &skybox);
    glDepthMask(GL_TRUE);

    glCullFace(OldCullFaceMode);
    glDepthFunc(OldDepthFuncMode);
}

glm::mat4 Render::GetPV() {
    return PV;
}

void Render::UpdatePV(bool perspective) {
    if (perspective)
        UpdatePV_Perspective();
    else
        UpdatePV_Orthographic();
}

void Render::UpdatePV_Perspective() {
    const auto& cam = pipeline.camera;

    View = glm::lookAt(
        cam.GetPosition(),
        cam.GetPosition() + cam.Params.Target, // направление взгляда
        cam.Params.Up
    );

    ProjTrans = glm::perspective(
        glm::radians(cam.PersProj.FOV),
        static_cast<float>(cam.PersProj.Width) / cam.PersProj.Height,
        cam.PersProj.zNear,
        cam.PersProj.zFar
    );

    PV = ProjTrans * View;
}

void Render::UpdatePV_Orthographic() {
    const auto& cam = pipeline.camera;

    View = glm::lookAt(
        cam.GetPosition(),
        cam.GetPosition() + cam.Params.Target,
        cam.Params.Up
    );

    float halfW = cam.PersProj.Width / 2.0f;
    float halfH = cam.PersProj.Height / 2.0f;

    ProjTrans = glm::ortho(
        -halfW, halfW,
        -halfH, halfH,
        cam.PersProj.zNear, cam.PersProj.zFar
    );

    PV = ProjTrans * View;
}
