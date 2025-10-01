#pragma once
#include "shader.hpp"
#include "texture.hpp"
#include <functional>
#include <variant>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/dual_quaternion.hpp>

class Material {
public:
    using MaterialValue = std::variant<
        int, float, GLuint,
        glm::vec2, glm::vec3, glm::vec4, glm::dualquat,
        glm::mat4,
        std::vector<glm::dualquat>, std::vector<glm::mat4>
    >;

    Material(Shader *shader);

    void Set(const std::string& name, const MaterialValue& v);
    void Apply() const;

    void Bind() const;
    Shader* GetShader() const;
    const std::vector<Texture*>& GetTexture() const;
    void SetShader(Shader *new_shader);
    void PushTexture(Texture *new_texture);
    void SetTexture(std::vector<Texture*>& new_texture);

    static Material* Create(const std::string& name, Shader *shader);
    static Material* Find(const std::string &name);
    static void Delete(const std::string &name);
    static void ClearСache();

    std::unordered_map<std::string, MaterialValue> values;

private:
    void UploadUniform(const GLint loc, const int v) const;
    void UploadUniform(const GLint loc, const float v) const;
    void UploadUniform(const GLint loc, const GLuint v) const;
    void UploadUniform(const GLint loc, const glm::vec2& v) const;
    void UploadUniform(const GLint loc, const glm::vec3& v) const;
    void UploadUniform(const GLint loc, const glm::vec4& v) const;
    void UploadUniform(const GLint loc, const glm::dualquat& v) const;
    void UploadUniform(const GLint loc, const glm::mat4& v) const;
    void UploadUniform(const GLint loc, const std::vector<glm::mat4>& v) const;
    void UploadUniform(const GLint loc, const std::vector<glm::dualquat>& v) const;

    Shader* shader = nullptr;
    std::vector<Texture*> texture;

    inline static std::unordered_map<std::string, Material> cache;
};
