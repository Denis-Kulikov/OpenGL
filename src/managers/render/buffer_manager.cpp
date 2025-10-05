#include <managers/render_manager.hpp>


void BufferManager::Init() {
    CreateBuffer("Matrices", BufferType::Uniform, BufferBinding::Matrices, sizeof(MatricesUBO));
    CreateBuffer("Lights", BufferType::Storage, BufferBinding::Lights, sizeof(LightsSSBO));
}

void BufferManager::CreateBuffer(const std::string& name, BufferType type, BufferBinding binding, GLsizeiptr size) {
    const std::unordered_map<BufferType, GLenum> gl_type = {
        {BufferType::Uniform, GL_UNIFORM_BUFFER}, {BufferType::Storage, GL_SHADER_STORAGE_BUFFER}
    };
    GLuint buffer;
    glGenBuffers(1, &buffer);


    GLenum target = gl_type.find(type)->second;
    glBindBufferBase(target, (GLuint)binding, buffer);
    glBufferData(target, size, nullptr, GL_DYNAMIC_DRAW);

    buffers[name] = {buffer, type, binding, size};
    binds[binding] = buffer;
}

void BufferManager::Update() {
    Update(BufferBinding::Matrices, MatricesUBO(RenderManager::pipeline.ProjTrans, RenderManager::pipeline.View));
}

GLuint BufferManager::GetBuffer(BufferBinding binding) {
    auto it = binds.find(binding);
    if (it != binds.end())
        return (GLuint)it->second;

    return -1;
}
