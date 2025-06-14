#include <object/material/material.hpp>

Material::Material(Shader *shader, InitFunction* initializer, ApplyFunction* applier)
    : shader(shader), initializer(initializer), applier(applier)
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
    shader->Bind();
    (*applier)(*this);
}

void Material::UpdateUniforms() {
    (*initializer)(*this);
}

Material* Material::Create(const std::string& name, Shader *shader, InitFunction* initializer, ApplyFunction* applier) {
    auto [it, inserted] = cache.try_emplace(name, shader, initializer, applier);
    
    if (inserted) {
        it->second.SetShader(shader);
    }

    return &it->second;
}

Material* Material::Find(const std::string &name) {
    auto it = cache.find(name);
    return it != cache.end() ? &it->second : nullptr;
}

void Material::Delete(const std::string &name) {
    auto it = cache.find(name);
    if (it != cache.end()) {
        if (it->second.initializer)
            delete it->second.initializer;
        if (it->second.applier)
            delete it->second.applier;
        cache.erase(it); 
    }
}

void Material::ClearСache() {
    for (auto it = cache.begin(); it != cache.end(); ) {
        if (it->second.initializer)
            delete it->second.initializer;
        if (it->second.applier)
            delete it->second.applier;
    }
}
