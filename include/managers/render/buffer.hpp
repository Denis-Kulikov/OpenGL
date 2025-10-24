#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <array>
#include <unordered_map>
#include <object/light/point_light.hpp>
#include <object/light/directional_light.hpp>

enum class BufferBinding : GLuint {
    Matrices = 0,
    Lights = 1,
    BonesData = 2,
    WrongBinding
};

enum class BufferType : GLuint {
    Uniform,
    Storage,
    WrongType
};

struct BufferInfo {
    BufferInfo(GLuint id, BufferType type, BufferBinding binding, GLsizeiptr size)
        : id(id), type(type), binding(binding), size(size)
    {}

    BufferInfo()
        : id(-1), type(BufferType::WrongType), binding(BufferBinding::WrongBinding), size(0)
    {}

    GLuint id;
    BufferType type;
    BufferBinding binding;
    GLsizeiptr size;
};

class BufferManager {
private:
    // BufferData
    struct MatricesUBO {
        MatricesUBO(const glm::mat4& Projection, const glm::mat4& View, const glm::vec3& viewPos)
            : Projection(Projection), View(View), viewPos(viewPos) {}

        glm::mat4 Projection;
        glm::mat4 View;
        glm::vec3 viewPos;
    };

    struct LightsSSBO {
        LightsSSBO(int numPointLights,
                   int numDirectionalLights,
                   int numSpotLights,
                   std::array<PointLight, 128> pointLights,
                   std::array<DirectionalLight, 16> dirLights
        )
        :   numPointLights(numPointLights),
            numDirectionalLights(numDirectionalLights),
            numSpotLights(numSpotLights),
            pointLights(pointLights),
            dirLights(dirLights)
        {}

        int numPointLights;
        int numDirectionalLights;
        int numSpotLights;
        int padding1;

        std::array<PointLight, 128> pointLights;
        std::array<DirectionalLight, 16> dirLights;
        // SpotLight spotLights[32];
    };

    struct BonesDataSSBO {
        static const std::size_t SIZE = 320;

        BonesDataSSBO(const std::vector<glm::mat4>& vec)
        {
            int i = 0;
            for (const auto& v : vec) {
                bones[i++] = v;
                if (i >= SIZE) break; 
            }
        }
        BonesDataSSBO(const std::array<glm::mat4, 320>& bones)
            : bones(bones)
        {}

        std::array<glm::mat4, SIZE> bones;
    };
    
public:
    std::unordered_map<std::string, BufferInfo> buffers;
    std::unordered_map<BufferBinding, BufferInfo> binds;

    void Init();
    void CreateBuffer(const std::string& name, BufferType type, BufferBinding binding, GLsizeiptr size);
    void Update();
    void UpdateLightsSSBO();
    void UpdateBonesDataSSBO(const std::vector<glm::mat4>& vec);
    GLuint GetBuffer(BufferBinding binding) const;
    BufferInfo GetBufferInfo(BufferBinding binding) const;

    template<typename T>
    void Update(BufferBinding binding, const T& data) {
        BufferInfo bInfo = GetBufferInfo(binding);
        if (bInfo.id == -1) {
            std::cout << "Error BufferManager::Update(...): handle == -1" << std::endl;
            return;
        }
        GLenum target = (bInfo.type == BufferType::Storage) 
                    ? GL_SHADER_STORAGE_BUFFER 
                    : GL_UNIFORM_BUFFER;

        glBindBuffer(target, bInfo.id);
        glBufferSubData(target, 0, sizeof(T), &data);
    }

    template<typename T>
    void Update(BufferBinding binding, const T* data, const std::size_t size) {
        BufferInfo bInfo = GetBufferInfo(binding);
        if (bInfo.id == -1) {
            std::cout << "Error BufferManager::Update(...): handle == -1" << std::endl;
            return;
        }
        GLenum target = (bInfo.type == BufferType::Storage) 
                    ? GL_SHADER_STORAGE_BUFFER 
                    : GL_UNIFORM_BUFFER;

        glBindBuffer(target, bInfo.id);
        glBufferSubData(target, 0, size, data);
    }
};
