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
#include <set>
#include <object/named.hpp>

class Shader;
class TextureUnit;

class Material : public Named {
public:
    using MaterialValue = std::variant<
        int, float, GLuint,
        glm::vec2, glm::vec3, glm::vec4, glm::dualquat, glm::mat4,
        std::vector<glm::vec4>, std::vector<glm::dualquat>, std::vector<glm::mat4>
    >;

    static inline std::set<std::string> ConfigurableFields = {
        "roughness",
        "metallic",
        "ambientStrength",
        "specularStrength"
    };


    Material(const std::string& name = "NoName");

    void Bind(const Shader* shader) const;
    void Set(const std::string& name, const MaterialValue& v);
    void UniformUIController(const std::string& name);
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

    void UniformUIController(const std::string& name, int& v);
    void UniformUIController(const std::string& name, float& v);
    void UniformUIController(const std::string& name, GLuint& v);
    void UniformUIController(const std::string& name, glm::vec2& v);
    void UniformUIController(const std::string& name, glm::vec3& v);
    void UniformUIController(const std::string& name, glm::vec4& v);
    void UniformUIController(const std::string& name, glm::dualquat& v);
    void UniformUIController(const std::string& name, glm::mat4& v);
    void UniformUIController(const std::string& name, std::vector<glm::vec4>& v);
    void UniformUIController(const std::string& name, std::vector<glm::mat4>& v);
    void UniformUIController(const std::string& name, std::vector<glm::dualquat>& v);

    inline static std::unordered_map<std::string, Material> cache;
};
