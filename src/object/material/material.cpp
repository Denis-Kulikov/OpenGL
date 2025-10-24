#include <object/material/material.hpp>


void Material::Bind(const Shader* shader) const {
    for (auto& [name, val] : values) {
        if (auto u = shader->FindUniform(name)) {
            std::visit([&](auto&& v) {
                UploadUniform(u->location, v);
            }, val);
        }
    }

    if (textureUnits.empty()) {
        glActiveTexture(GL_TEXTURE0);
        Texture::Find("white")->Bind();
    }

    for (const auto& u : textureUnits) {
        u.Bind();
    }
}

void Material::Set(const std::string& name, const MaterialValue& v) {
    values[name] = v;
}

bool Material::LinkTextureUnits(const Shader* shader) {
    GLuint unitIndex = 0;
    for (auto& t : textureUnits) {
        const auto texName = TextureUnit::Types.find(t.type);
        if (texName == TextureUnit::Types.end())
            return false;

        const auto loc = shader->uniforms.find(texName->second);
        if (loc == shader->uniforms.end())
            return false;

        t.unit = unitIndex;
        Set(texName->second, unitIndex);

        ++unitIndex;
    }

    return true;
}

void Material::UploadUniform(const GLint loc, const int v) const { glUniform1i(loc, v); }
void Material::UploadUniform(const GLint loc, const float v) const { glUniform1f(loc, v); }
void Material::UploadUniform(const GLint loc, const GLuint v) const { glUniform1i(loc, v); }
void Material::UploadUniform(const GLint loc, const glm::vec2& v) const { glUniform2fv(loc, 1, glm::value_ptr(v)); }
void Material::UploadUniform(const GLint loc, const glm::vec3& v) const { glUniform3fv(loc, 1, glm::value_ptr(v)); }
void Material::UploadUniform(const GLint loc, const glm::vec4& v) const { glUniform4fv(loc, 1, glm::value_ptr(v)); }
void Material::UploadUniform(const GLint loc, const glm::dualquat& v) const { glUniform4fv(loc, 2, glm::value_ptr(v.real)); }
void Material::UploadUniform(const GLint loc, const glm::mat4& v) const { glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(v)); }
void Material::UploadUniform(const GLint loc, const std::vector<glm::vec4>& v) const {
    glUniform4fv(loc, v.size(), glm::value_ptr(v.data()[0]));
}
void Material::UploadUniform(const GLint loc, const std::vector<glm::mat4>& v) const {
    glUniformMatrix4fv(loc, v.size(), GL_FALSE, glm::value_ptr(v.data()[0]));
}
void Material::UploadUniform(const GLint loc, const std::vector<glm::dualquat>& v) const {
    glUniform4fv(loc, v.size() * 2, glm::value_ptr(v.data()[0].real));
}

Material* Material::Create(const std::string& name) {
    auto [it, inserted] = cache.try_emplace(name);
    
    if (inserted) {
        // it->second.SetShader(shader); !!!
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
        cache.erase(it); 
    }
}

void Material::ClearСache() {
    for (auto it = cache.begin(); it != cache.end(); ) {
    }
}
