#include <render/render.hpp>

Render::Render(Pipeline &_pipeline)
{
    pipeline.camera = _pipeline.camera;
    pipeline.object = _pipeline.object;
}

Render::Render(Camera *_camera)
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


void Render::drawObject(objectTransform &_transform, Sprite&_sprite)
{
    pipeline.object = &_transform;
    // pipeline.object->print();
    if ((pipeline.camera == nullptr) || (pipeline.object == nullptr)) {
        std::cout << "Error Render.drawObject(): " << std::endl;
        if (pipeline.camera == nullptr) std::cout << "Not found Camera ";
        if (pipeline.object == nullptr) std::cout << "Not found ObjectTransform ";
        std::cout << std::endl;
        return;
    }

    glUseProgram(_sprite.shader);
    if (_sprite.gColorLocation != 0xFFFFFFFF) glUniform3f(_sprite.gColorLocation, _sprite.color.x, _sprite.color.y, _sprite.color.z);

    glUniformMatrix4fv(_sprite.gWorldLocation, 1, GL_TRUE, pipeline.GetGLMatrix());

    glBindVertexArray(_sprite.geometry->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _sprite.geometry->VBO);

    if (_sprite.geometry->EBO != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _sprite.geometry->EBO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _sprite.texture);
        glUniform1i(_sprite.gTextureSamplerLocation, 0);

        glDrawElements(GL_TRIANGLES, _sprite.geometry->numIndices, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_LINE_STRIP, 0, _sprite.geometry ->numVertices);
    }


    clearRender();
}
