#pragma once
#include <functional>
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include "texture_unit.hpp"
#include <variant>
#include <vector>

class Shader;
class TextureUnit;

class Material {
public:
    using MaterialValue = std::variant<
        int, float, GLuint,
        glm::vec2, glm::vec3, glm::vec4, glm::dualquat, glm::mat4,
        std::vector<glm::vec4>, std::vector<glm::dualquat>, std::vector<glm::mat4>
    >;

    void Bind(const Shader* shader) const;
    void BindShadowPass(const Shader* shader, const glm::mat4& model, const glm::mat4& shadowProj) const;
    void Set(const std::string& name, const MaterialValue& v);
    GLuint LinkTextureUnits(GLuint unitIndex, const Shader* shader);
    GLuint PushTextureUnits(GLuint unitIndex, TextureUnit* unit, const Shader* shader);

    static Material* Create(const std::string& name);
    static Material* Find(const std::string &name);
    static void Delete(const std::string &name);
    static void ClearСache();

    std::unordered_map<std::string, MaterialValue> values;
    std::vector<TextureUnit> textureUnits;

private:
    void UploadUniform(const GLint loc, const int v) const;
    void UploadUniform(const GLint loc, const float v) const;
    void UploadUniform(const GLint loc, const GLuint v) const;
    void UploadUniform(const GLint loc, const glm::vec2& v) const;
    void UploadUniform(const GLint loc, const glm::vec3& v) const;
    void UploadUniform(const GLint loc, const glm::vec4& v) const;
    void UploadUniform(const GLint loc, const glm::dualquat& v) const;
    void UploadUniform(const GLint loc, const glm::mat4& v) const;
    void UploadUniform(const GLint loc, const std::vector<glm::vec4>& v) const;
    void UploadUniform(const GLint loc, const std::vector<glm::mat4>& v) const;
    void UploadUniform(const GLint loc, const std::vector<glm::dualquat>& v) const;

    inline static std::unordered_map<std::string, Material> cache;
};
