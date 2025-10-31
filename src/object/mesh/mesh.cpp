#include <object/mesh/mesh.hpp>
#include <managers/global.hpp>
#include <object/material/shader.hpp>
#include <object/material/material.hpp>
#include <object/material/texture_unit.hpp>


Mesh::Mesh(const MeshData& meshData, Shader* shader)
    : shader(shader)
{
    InitBuffers();

    glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(meshData.Indices[0]) * meshData.Indices.size(), meshData.Indices.data(), GL_STATIC_DRAW);

    // Добавление массивов вершинных атрибутов
    AddAttribute(meshData.Positions.data(), meshData.Positions.size() * sizeof(glm::vec3), "aPosition", 3);
    AddAttribute(meshData.TexCoords.data(), meshData.TexCoords.size() * sizeof(glm::vec2), "aTexCoord", 2);
    AddAttribute(meshData.Normals.data(), meshData.Normals.size() * sizeof(glm::vec3), "aNormal", 3); // нет в шейдере
    // AddAttribute(meshData.Tangent.data(), meshData.Tangent.size() * sizeof(glm::vec3), "aTangent", 3);
    // AddAttribute(meshData.Bitangent.data(), meshData.Bitangent.size() * sizeof(glm::vec3), "aBitangent", 3);
    // AddAttribute(meshData.Color.data(), meshData.Color.size() * sizeof(glm::vec3), "aColor", 3);
    // AddAttribute(meshData.InstanceID.data(), meshData.InstanceID.size() * sizeof(float), "aInstanceID", 1);
    // AddAttribute(meshData.InstanceMatrix.data(), meshData.InstanceMatrix.size() * sizeof(glm::mat4), "aInstanceMatrix", 16);

    material = std::make_shared<Material>();
    m_Entries.reserve(meshData.m_Entries.size());
    for (const auto& m : meshData.m_Entries) {
        m_Entries.emplace_back(m.NumIndices, m.BaseVertex, m.BaseIndex, m.material.get());
    }
    LinkUniforms();

    GLuint unitIndex = 0;
    Texture* tex = Texture::Find("ShadowMapPoint");
    auto texName = TextureUnit::Types.at(TextureUnit::TextureType::SHADOW_MAP_CUBE);

    auto loc = shader->FindUniform(texName);
    if (loc == nullptr) {
        std::cout << "Material::LinkTextureUnits(...): not found textre location \"" << texName << "\"" << std::endl;
    } else {
        TextureUnit tUnit(tex, TextureUnit::TextureType::SHADOW_MAP_CUBE);
        tUnit.unit = unitIndex;
        material->textureUnits.push_back(tUnit);
        material->Set(texName, unitIndex);
        ++unitIndex;
    }

    
    tex = Texture::Find("ShadowMapDirLight");
    texName = TextureUnit::Types.at(TextureUnit::TextureType::SHADOW_MAP_DIR);

    loc = shader->FindUniform(texName);
    if (loc == nullptr) {
        std::cout << "Material::LinkTextureUnits(...): not found textre location \"" << texName << "\"" << std::endl;
    } else {
        TextureUnit tUnit(tex, TextureUnit::TextureType::SHADOW_MAP_DIR);
        tUnit.unit = unitIndex;
        material->textureUnits.push_back(tUnit);
        material->Set(texName, unitIndex);
        ++unitIndex;
    }

    for (auto& m : m_Entries) {
        m.material->LinkTextureUnits(unitIndex, shader);
    }

    glBindVertexArray(0);	
}

void Mesh::LinkUniforms() {
    auto& values = material->values;

    for (const auto& [name, info] : shader->uniforms)
    {
        std::string uniformName = name;
        if (uniformName.size() > 3) {
            if (uniformName.substr(uniformName.size() - 3) == "[0]") {
                uniformName = uniformName.substr(0, uniformName.size() - 3);
            }
        }

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
}

Shader* Mesh::GetShader() const {
    return shader;
}

void Mesh::InitBuffers() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);

    vbos.resize(shader->attributes.size());
    buffers.resize(shader->attributes.size());
    glGenBuffers(shader->attributes.size(), buffers.data());
}

void Mesh::AddAttribute(const void* data, size_t size, const std::string& attrName, GLint sizePerVertex,
                GLenum type, GLboolean normalized, GLsizei stride, size_t offset)
{
    if (!data || size == 0) return;

    auto attrInfo = shader->attributes.find(attrName);
    if (attrInfo == shader->attributes.end()) {
        // std::cout << "Error Mesh::AddAttribute(): " << attrName << " not found" << std::endl;
        return;
    }
    
    auto attr = attrInfo->second.attribute;
    auto loc = attrInfo->second.location;

    glBindBuffer(GL_ARRAY_BUFFER, buffers[attr]);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(loc);

    if (type == GL_INT || type == GL_UNSIGNED_INT) {
        glVertexAttribIPointer(loc, sizePerVertex, type, stride, (const GLvoid*)offset);
    } else {
        glVertexAttribPointer(loc, sizePerVertex, type, normalized, stride, (const GLvoid*)offset);

    }

    vbos.push_back({buffers[attr], attr, sizePerVertex, type, normalized, stride, offset});
}

void Mesh::Bind() const {
    shader->Bind();
    BindGeometry();
    material->Bind(GetShader());
}
void Mesh::BindGeometry() const {
    glBindVertexArray(vao);
}


Mesh* Mesh::Create(const std::string& name, const MeshData& meshData, Shader* shader) {
    auto [it, inserted] = cache.try_emplace(name, meshData, shader);
    return &it->second;
}
Mesh* Mesh::Find(const std::string& name) {
    auto it = cache.find(name);
    return it != cache.end() ? &it->second : nullptr;
}
void Mesh::Delete(const std::string& path) {
    auto it = cache.find(path);
    if (it != cache.end()) {
        cache.erase(it); 
    }
}
void Mesh::ClearСache() {
    for (auto it = cache.begin(); it != cache.end(); ) {
        it = cache.erase(it);
    }
}
