#include <object/material/material.hpp>

Material::Material(Shader* shader)
    : shader(shader)
{
    for (const auto& [name, info] : shader->uniforms)
    {
        std::string uniformName = name;
        if (uniformName.size() > 3) {
            if (uniformName.substr(uniformName.size() - 3) == "[0]") {
                uniformName = uniformName.substr(0, uniformName.size() - 3);
            }
        }

        // std::cout << "Uniform: " << uniformName << std::endl;
        switch (info.type)
        {
            case GL_INT:
            case GL_BOOL:
                values[uniformName] = int(0);
                break;

            case GL_FLOAT:
                values[uniformName] = float(0.0f);
                break;

            case GL_SAMPLER_2D:
            case GL_SAMPLER_CUBE:
                values[uniformName] = GLuint(0); // texture unit index
                break;

            case GL_FLOAT_VEC2:
                values[uniformName] = glm::vec2(0.0f);
                break;

            case GL_FLOAT_VEC3:
                values[uniformName] = glm::vec3(0.0f);
                break;

            case GL_FLOAT_VEC4:
                if (uniformName.size() >= 2 && uniformName.substr(0, 2) == "DQ") { // dualquat
                    if (info.size > 2) {
                        values[uniformName] = std::vector<glm::dualquat>(info.size, glm::dualquat({1, 0, 0, 0}, {0, 0, 0, 0}));
                    } else {
                        values[uniformName] = glm::dualquat({1, 0, 0, 0}, {0, 0, 0, 0});
                    }
                } else { // vec4
                    if (info.size > 1) {
                        values[uniformName] = std::vector<glm::vec4>(info.size, glm::vec4(0.0f));
                    } else {
                        values[uniformName] = glm::vec4(0.0f);
                    }
                }


                break;

            case GL_FLOAT_MAT4:
                if (info.size > 1) {
                    values[uniformName] = std::vector<glm::mat4>(info.size, glm::mat4(1.0f));
                } else {
                    values[uniformName] = glm::mat4(1.0f);
                }
                break;

            default:
                std::cerr << "[Material] Unsupported uniform type for " << uniformName << " (GLenum=" << info.type << ")\n";
                break;
        }
    }
    std::cout << std::endl;
}

Shader* Material::GetShader() const {
    return shader;
}

void Material::SetShader(Shader *new_shader) {
    shader = new_shader;
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
    GetShader()->Bind();
    glActiveTexture(GL_TEXTURE0);
    Apply();
}

void Material::Set(const std::string& name, const MaterialValue& v) {
    values[name] = v;
}

void Material::Apply() const {
    glUseProgram(shader->GetID());
    for (auto& [name, val] : values) {
        if (auto u = shader->FindUniform(name)) {
            std::visit([&](auto&& v) {
                UploadUniform(u->location, v);
            }, val);
        }
    }
}

Material* Material::Create(const std::string& name, Shader *shader) {
    auto [it, inserted] = cache.try_emplace(name, shader);
    
    if (inserted) {
        it->second.SetShader(shader);
    }

    return &it->second;
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
