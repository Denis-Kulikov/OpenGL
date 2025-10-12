#include <object/mesh/mesh.hpp>
#include <managers/global_state.hpp>


Mesh::Mesh(const MeshData& meshData, Shader* shader)
    : m_Entries(meshData.m_Entries), shader(shader)
{
    InitBuffers();

    glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(meshData.Indices[0]) * meshData.Indices.size(), meshData.Indices.data(), GL_STATIC_DRAW);

    LinkUniforms();

    // Добавление массивов вершинных атрибутов
    AddAttribute(meshData.Positions.data(), meshData.Positions.size() * sizeof(glm::vec3), "aPosition", 3);
    AddAttribute(meshData.TexCoords.data(), meshData.TexCoords.size() * sizeof(glm::vec2), "aTexCoord", 2);
    AddAttribute(meshData.Normals.data(), meshData.Normals.size() * sizeof(glm::vec3), "aNormal", 3); // нет в шейдере
    // AddAttribute(meshData.Tangent.data(), meshData.Tangent.size() * sizeof(glm::vec3), "aTangent", 3);
    // AddAttribute(meshData.Bitangent.data(), meshData.Bitangent.size() * sizeof(glm::vec3), "aBitangent", 3);
    // AddAttribute(meshData.Color.data(), meshData.Color.size() * sizeof(glm::vec3), "aColor", 3);
    // AddAttribute(meshData.InstanceID.data(), meshData.InstanceID.size() * sizeof(float), "aInstanceID", 1);
    // AddAttribute(meshData.InstanceMatrix.data(), meshData.InstanceMatrix.size() * sizeof(glm::mat4), "aInstanceMatrix", 16);

    for (auto& m : m_Entries) {
        GLuint unit = 0;
        for (auto& t : m.Material.textureUnits) {
            if (t.Link(GetShader(), unit))
            ++unit;
        }
    }

    glBindVertexArray(0);	
}

void Mesh::LinkUniforms() {
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
                material.values[uniformName] = int(0);
                break;

            case GL_FLOAT:
                material.values[uniformName] = float(0.0f);
                break;

            case GL_SAMPLER_2D:
            case GL_SAMPLER_CUBE:
                material.values[uniformName] = GLuint(0); // texture unit index
                break;

            case GL_FLOAT_VEC2:
                material.values[uniformName] = glm::vec2(0.0f);
                break;

            case GL_FLOAT_VEC3:
                material.values[uniformName] = glm::vec3(0.0f);
                break;

            case GL_FLOAT_VEC4:
                if (uniformName.size() >= 2 && uniformName.substr(0, 2) == "DQ") { // dualquat
                    if (info.size > 2) {
                        material.values[uniformName] = std::vector<glm::dualquat>(info.size, glm::dualquat({1, 0, 0, 0}, {0, 0, 0, 0}));
                    } else {
                        material.values[uniformName] = glm::dualquat({1, 0, 0, 0}, {0, 0, 0, 0});
                    }
                } else { // vec4
                    if (info.size > 1) {
                        material.values[uniformName] = std::vector<glm::vec4>(info.size, glm::vec4(0.0f));
                    } else {
                        material.values[uniformName] = glm::vec4(0.0f);
                    }
                }


                break;

            case GL_FLOAT_MAT4:
                if (info.size > 1) {
                    material.values[uniformName] = std::vector<glm::mat4>(info.size, glm::mat4(1.0f));
                } else {
                    material.values[uniformName] = glm::mat4(1.0f);
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
        std::cout << "Error Mesh::AddAttribute(): " << attrName << " not found" << std::endl;
        return;
    }
    
    auto attr = attrInfo->second.attribute;

    glBindBuffer(GL_ARRAY_BUFFER, buffers[attr]);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attr);

    if (type == GL_INT || type == GL_UNSIGNED_INT) {
        glVertexAttribIPointer(attr, sizePerVertex, type, stride, (const GLvoid*)offset);
    } else {
        glVertexAttribPointer(attr, sizePerVertex, type, normalized, stride, (const GLvoid*)offset);
    }

    vbos.push_back({buffers[attr], attr, sizePerVertex, type, normalized, stride, offset});
}

void Mesh::Bind() const {
    shader->Bind();
    glBindVertexArray(vao);
    material.Bind(GetShader());
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
