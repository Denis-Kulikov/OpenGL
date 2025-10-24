#include <managers/render/render.hpp>


void BufferManager::Init() {
    CreateBuffer("Matrices", BufferType::Uniform, BufferBinding::Matrices, sizeof(MatricesUBO));
    CreateBuffer("Lights", BufferType::Storage, BufferBinding::Lights, sizeof(LightsSSBO));
    CreateBuffer("BonesData", BufferType::Storage, BufferBinding::BonesData, sizeof(BonesDataSSBO));

    UpdateLightsSSBO();
}

void BufferManager::CreateBuffer(const std::string& name, BufferType type, BufferBinding binding, GLsizeiptr size) {
    static const std::unordered_map<BufferType, GLenum> gl_type = {
        {BufferType::Uniform, GL_UNIFORM_BUFFER},
        {BufferType::Storage, GL_SHADER_STORAGE_BUFFER}
    };

    GLuint buffer;
    GLenum target = gl_type.at(type);
    
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(target, (GLuint)binding, buffer);
    glBindBuffer(target, 0);

    buffers[name] = {buffer, type, binding, size};
    binds[binding] = {buffer, type, binding, size};
}

void BufferManager::Update() {
    Update(BufferBinding::Matrices, 
        MatricesUBO(RenderManager::pipeline.ProjTrans,
                    RenderManager::pipeline.View,
                    RenderManager::pipeline.camera->GetPosition()
    ));
}

void BufferManager::UpdateLightsSSBO() {
    std::array<PointLight, 128> pl;
    std::array<DirectionalLight, 16> dl;
    pl[0] = PointLight(glm::vec4(0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f), 100.0f);
    dl[0] = DirectionalLight(glm::vec4(0.0f, -1.0f, 0.0f, 0.0f), glm::vec3(1.0f), 0.5f);
    Update(BufferBinding::Lights, LightsSSBO(1, 0, 0, pl, dl));
}

void BufferManager::UpdateBonesDataSSBO(const std::vector<glm::mat4>& vec) {
    Update(BufferBinding::BonesData, vec.data(), std::min(vec.size(), BonesDataSSBO::SIZE) * sizeof(glm::mat4));
}


GLuint BufferManager::GetBuffer(BufferBinding binding) const {
    auto it = binds.find(binding);
    if (it != binds.end())
        return (GLuint)it->second.id;

    return -1;
}

BufferInfo BufferManager::GetBufferInfo(BufferBinding binding) const {
    auto it = binds.find(binding);
    if (it != binds.end())
        return it->second;

    return BufferInfo();
}
