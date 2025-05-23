#include <object/material/material.hpp>

Material::Material(const std::string& name, InitFunction* initializer, ApplyFunction* applier)
    : name(name), initializer(initializer), applier(applier)
{}


Shader* Material::GetShader() const {
    return shader;
}

void Material::SetShader(Shader *new_shader) {
    shader = new_shader;
    UpdateUniforms();
}

const std::vector<Texture*>& Material::GetTexture() const {
    return texture;
}

void Material::PushTexture(Texture *new_texture) {
    texture.push_back(new_texture);
}

void Material::SetTexture(std::vector<Texture*>& new_texture) {
    texture = new_texture;
}

void Material::Bind() const {
    (*applier)(*this);
}

void Material::UpdateUniforms() {
    (*initializer)(*this);
}

Material* Material::Create(const std::string& name, Shader *shader, InitFunction* initializer, ApplyFunction* applier) {
    auto [it, inserted] = materialChashe.try_emplace(name, name, initializer, applier);
    
    if (inserted) {
        it->second.SetShader(shader);
    }

    return &it->second;
}

void Material::Delete(const std::string &name) {
}

Material* Material::Find(const std::string &name) {
    auto it = materialChashe.find(name);
    return it != materialChashe.end() ? &it->second : nullptr;
}
