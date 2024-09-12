#include <render/render.hpp>

#include <chrono>
extern std::chrono::milliseconds totalTime;

Render::Render(Pipeline &_pipeline)
{
    pipeline.camera = _pipeline.camera;
}

Render::Render(Camera *_camera)
{
    SetCamera(_camera);
}

Render::Render()
{
    SetCamera(nullptr);
}

void Render::SetCamera(Camera *_camera)
{
    pipeline.camera = _camera;
}

void Render::clearRender() {
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    shader = 0;
    texture = 0;
}

void Render::PushGeometry(struct GeometryInfo *geometry)
{
    glBindVertexArray(geometry->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->EBO);
}

void Render::drawObject(Matrix4f<GLfloat>& matrix, Sprite *sprite)
{
    if ((pipeline.camera == nullptr) || (sprite == nullptr)) {
        std::cout << "Error Render.drawObject(): " << std::endl;
        if (pipeline.camera == nullptr) std::cout << "Not found Camera ";
        if (sprite == nullptr)          std::cout << "Not found Sprite ";
        std::cout << std::endl;
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

    if (sprite->gColorLocation != 0xFFFFFFFF) glUniform3f(sprite->gColorLocation, sprite->color.x, sprite->color.y, sprite->color.z);
    glUniformMatrix4fv(sprite->gWorldLocation, 1, GL_TRUE, &matrix);

    if (sprite->GetGeometry()->EBO != 0) {
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->EBO);
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
    skybox_transform.SetWorldPos(pipeline.camera->GetPosition());
    skybox_transform.SetRotate(Vector3<GLfloat>(0.0, 90.0, 180));
    
    glDepthMask(GL_FALSE);
    drawObject(pipeline.GetTransform(skybox_transform), &skybox);
    glDepthMask(GL_TRUE);

    glCullFace(OldCullFaceMode);
    glDepthFunc(OldDepthFuncMode);
}
