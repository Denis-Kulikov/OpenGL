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
}

void Render::PushGeometry(struct GeometryInfo *geometry)
{
    glBindVertexArray(geometry->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->EBO);
}

void Render::drawObject(objectTransform *transform, Sprite *sprite) const
{
    if ((pipeline.camera == nullptr) || (transform == nullptr) || (sprite == nullptr)) {
        std::cout << "Error Render.drawObject(): " << std::endl;
        if (pipeline.camera == nullptr) std::cout << "Not found Camera ";
        if (transform == nullptr) std::cout << "Not found ObjectTransform ";
        if (sprite == nullptr)         std::cout << "Not found Sprite ";
        std::cout << std::endl;
        return;
    }

    glUseProgram(sprite->shader);
    if (sprite->gColorLocation != 0xFFFFFFFF) glUniform3f(sprite->gColorLocation, sprite->color.x, sprite->color.y, sprite->color.z);

    glUniformMatrix4fv(sprite->gWorldLocation, 1, GL_TRUE, reinterpret_cast<GLfloat*>(&pipeline.GetTransform(transform)));

    if (sprite->GetGeometry()->EBO != 0) {
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->EBO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sprite->texture);
        glUniform1i(sprite->gTextureSamplerLocation, 0);
        glDrawElements(GL_TRIANGLES, sprite->GetGeometry()->numIndices, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_LINE_STRIP, 0, sprite->GetGeometry()->numVertices);
    }

    // clearRender();
}
