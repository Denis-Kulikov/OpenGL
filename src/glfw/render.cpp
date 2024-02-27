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

void Render::drawObeject(sprite &_sprite)
{
    glUseProgram(_sprite.shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _sprite.texture);
    glUniform1i(_sprite.gTextureSamplerLocation, 0);
    if (_sprite.gColorLocation != 0xFFFFFFFF) glUniform3f(_sprite.gColorLocation, _sprite.color.x, _sprite.color.y, _sprite.color.z);

    pipeline.object = &_sprite.trans;
    if (pipeline.camera == nullptr) {
        std::cout << "Not found Camera" << std::endl;
        return;
    }
    if (pipeline.object == nullptr) {
        std::cout << "Not found ObjectTransform" << std::endl;
        return;
    }

    GLfloat floatMatrix[16];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            floatMatrix[i * 4 + j] = static_cast<GLfloat>(pipeline.GetTrans()->m[i][j]);
        }
    }
    glUniformMatrix4fv(_sprite.gWorldLocation, 1, GL_TRUE, floatMatrix);

    glBindVertexArray(_sprite.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _sprite.VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _sprite.EBO);

    if (_sprite.EBO != 0) {
        glDrawElements(GL_TRIANGLES, _sprite.numIndices, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_LINE_STRIP, 0, _sprite.numVertices);
    }

    glBindVertexArray(0);

    clearRender();
}
