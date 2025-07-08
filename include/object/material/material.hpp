#pragma once
#include "shader.hpp"
#include "texture.hpp"
#include <functional>
#include <glm/gtc/type_ptr.hpp>

class Material {
public:
    using InitFunction   = std::function<void(Material&)>;
    using ApplyFunction  = std::function<void(const Material&)>;

    Material(Shader *shader, InitFunction* initializer, ApplyFunction* applier);


    template<typename T>
    void UpdateValue(const std::string &uniform_name, const T& new_value) {
        void* ptr = values[uniform_name].second;
        if (ptr) {
            *static_cast<T*>(ptr) = new_value;
        }
    }

    void Bind() const;
    Shader* GetShader() const;
    const std::vector<Texture*>& GetTexture() const;
    void SetShader(Shader *new_shader);
    void PushTexture(Texture *new_texture);
    void SetTexture(std::vector<Texture*>& new_texture);

    static Material* Create(const std::string& name, Shader *shader, InitFunction* initializer, ApplyFunction* applier);
    static Material* Find(const std::string &name);
    static void Delete(const std::string &name);
    static void ClearСache();

    std::unordered_map<std::string, std::pair<GLint, void*>> values;

private:
    void UpdateUniforms();

    Shader* shader = nullptr;
    std::vector<Texture*> texture;
    InitFunction* initializer = nullptr; // утечка памяти
    ApplyFunction* applier = nullptr; // утечка памяти

    inline static std::unordered_map<std::string, Material> cache;
};
