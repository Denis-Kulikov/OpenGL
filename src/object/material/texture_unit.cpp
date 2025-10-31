#include <object/material/texture_unit.hpp>
#include <object/material/texture.hpp>


TextureUnit::TextureUnit(Texture* texture, TextureType type)
    : texture(texture), type(type)
{}

void TextureUnit::Bind() const {
    glActiveTexture(GL_TEXTURE0 + unit);
    texture->Bind();
}
