#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <unordered_map>

enum class BufferBinding : GLuint {
    Matrices = 0,
    Lights = 1
};

enum class BufferType : GLuint {
    Uniform,
    Storage
};

struct BufferInfo {
    GLuint id;
    BufferType type;
    BufferBinding binding;
    GLsizeiptr size;
};

class BufferManager {
private:
    // BufferData
    struct MatricesUBO {
        MatricesUBO(const glm::mat4& Projection, const glm::mat4& View)
            : Projection(Projection), View(View) {}

        glm::mat4 Projection;
        glm::mat4 View;
    };
    struct LightsSSBO {
    };
    
public:
    GLuint uboMatrices;
    GLuint uboLights;

    std::unordered_map<std::string, BufferInfo> buffers;
    std::unordered_map<BufferBinding, GLuint> binds;

    void Init();
    void CreateBuffer(const std::string& name, BufferType type, BufferBinding binding, GLsizeiptr size);
    void Update();
    GLuint GetBuffer(BufferBinding binding);

    template<typename T>
    void Update(BufferBinding binding, const T& data) {
        GLuint handle = GetBuffer(binding);
        if (handle == -1) {
            std::cout << "Error BufferManager::Update(...): handle == -1" << std::endl;
            return;
        }
        glBindBuffer(GL_UNIFORM_BUFFER, handle);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &data);
    }
};
