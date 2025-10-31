#include <object/material/material.hpp>
#include <object/material/shader.hpp>
#include <object/material/texture_unit.hpp>
#include <object/material/texture.hpp>


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
        // if (textureUnits.size() > 1) {
        //     std::cout << "texture id: " <<  u.texture->GetID() << " | unit: " << u.unit << std::endl;
        // }
        // std::cout << "texture id: " <<  u.texture->GetID() << " | unit: " << u.unit << std::endl;

        if (u.type == TextureUnit::TextureType::SHADOW_MAP_CUBE) {
            glActiveTexture(GL_TEXTURE0 + u.unit);
            glBindTexture(GL_TEXTURE_CUBE_MAP, u.texture->GetID());
        } else {
            u.Bind();
        }
    }
            // std::cout << std::endl;

        // if (textureUnits.size() > 1) {
        //     std::cout << std::endl;
        // }
}

#include <managers/global.hpp> 
#include <scene/scene.hpp>
void Material::BindShadowPass(const Shader* shader, const glm::mat4& model, const glm::mat4& shadowProj) const {
    auto mLoc = shader->FindUniform("Model")->location;
    auto pLoc = shader->FindUniform("ShadowProj")->location;
    auto lLoc = shader->FindUniform("lightPos")->location;
    auto fLoc = shader->FindUniform("farPlane")->location;

    UploadUniform(mLoc, model);
    UploadUniform(pLoc, shadowProj);
    UploadUniform(lLoc, GlobalState::scene->lighting.DataSSBO.pointLights[0].position);
    UploadUniform(fLoc, GlobalState::scene->shadow.pointLights.GetFar());
}

void Material::Set(const std::string& name, const MaterialValue& v) {
    values[name] = v;
}

GLuint Material::LinkTextureUnits(GLuint unitIndex, const Shader* shader) {
    for (auto& t : textureUnits) {
        if (t.unit != TextureUnit::NotActivated) continue;

        const auto texName = TextureUnit::Types.find(t.type);
        if (texName == TextureUnit::Types.end())
            return unitIndex;

        const auto loc = shader->FindUniform(texName->second);
        if (loc == nullptr) {
            std::cout << "Material::LinkTextureUnits(...): not found textre location \"" << texName->second << "\"" << std::endl;
            return unitIndex;
        }

        t.unit = unitIndex;
        Set(texName->second, unitIndex);

        ++unitIndex;
    }

    return unitIndex;
}

GLuint Material::PushTextureUnits(GLuint unitIndex, TextureUnit* unit, const Shader* shader) {
    if (unit->unit != TextureUnit::NotActivated) return unitIndex;

    const auto texName = TextureUnit::Types.find(unit->type);
    if (texName == TextureUnit::Types.end())
        return unitIndex;

    const auto loc = shader->FindUniform(texName->second);
    if (loc == nullptr) {
        std::cout << "Material::LinkTextureUnits(...): not found textre location \"" << texName->second << "\"" << std::endl;
        return unitIndex;
    }

    unit->unit = unitIndex;
    Set(texName->second, unitIndex);
    textureUnits.push_back(*unit);

    ++unitIndex;

    return unitIndex;
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
