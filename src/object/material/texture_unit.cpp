#include <object/material/texture_unit.hpp>


TextureUnit::TextureUnit(Texture* texture, TextureType type)
    : texture(texture), type(type)
{}

void TextureUnit::Bind() const {
    glActiveTexture(GL_TEXTURE0 + unit);
    texture->Bind();
}

// не корректная логика
// glUniform1i перебивается Material::Bind в ComponentMesh::Render
bool TextureUnit::Link(Shader* shader, GLuint i) {
    const auto texName = Types.find(type);
    if (texName == Types.end())
        return false;

    const auto loc = shader->uniforms.find(texName->second);
    if (loc == shader->uniforms.end())
        return false;

    unit = i;
    glUniform1i(loc->second.location, unit);

    return true;
}
