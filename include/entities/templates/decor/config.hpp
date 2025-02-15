#pragma once

#include "../../pawn.hpp"
#include <object/cube.hpp>
#include <object/sphere.hpp>
#include <object/component/template/shape.hpp>
#include <object/component/template/mesh.hpp>

class Config : public Pawn
{
public:
    Config();
    ~Config();
    
    static void Initialize();
    static void Deinitialization();

    std::string GetName() const override;
    Mesh *GetMesh() const override;

    void CreateScene() {
        rootComponent->children.clear();

        AttachToRoot<sprite_obj>();
        AttachToRoot<cube_obj>();
        AttachToRoot<sphere_obj>();
        AttachToRoot<mesh_obj>();

        // rootComponent->FixLocationTree();

        auto vec = scene.get_vector<mesh_obj>();
        for (auto &it : vec) {
            auto c = dynamic_cast<ComponentMesh*>(it.component);
            c->mesh->set_transform(c->GetMatrix());
        }

        rootComponent->UpdateMatrixTree();
    }

    void RenderScene() {
        RenderInstanced(scene.get_vector<sprite_obj>());
        RenderInstanced(scene.get_vector<cube_obj>());
        RenderInstanced(scene.get_vector<sphere_obj>());
        
        auto vec = scene.get_vector<mesh_obj>();
        for (const auto &it : vec) {
            dynamic_cast<ComponentMesh*>(it.component)->Render();
        }
    }

    inline static const int sphere_quality = 12;

protected:
    // Компоненты, у которых нет родителей, добавляются на сцену
    template<typename T>
    void AttachToRoot() {
        auto vec = scene.get_vector<T>();
        for (auto &it : vec) {
            if (it.component->parent == nullptr) {
                rootComponent->AddChild(it.component);
            }
        }
    }

    template<typename T>
    void RenderInstanced(const T& vec) {
        if (vec.empty()) return;

        std::vector<glm::mat4> modelMatrices;
        for (const auto& obj : vec) {
            modelMatrices.push_back(obj.component->GetMatrix());
        }

        static GLuint instanceVBO = 0;
        if (instanceVBO == 0) {
            glGenBuffers(1, &instanceVBO);
        }

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), modelMatrices.data(), GL_DYNAMIC_DRAW);

        glBindVertexArray(vec[0].shape->GetGeometry()->VAO);

        GLsizei vec4Size = sizeof(glm::vec4);
        for (int i = 0; i < 4; i++) {
            glEnableVertexAttribArray(3 + i); // Аттрибуты 3, 4, 5, 6 (четыре vec4, составляющие mat4)
            // glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * vec4Size));
            glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(intptr_t)(i * vec4Size));
            glVertexAttribDivisor(3 + i, 1); // Один аттрибут на экземпляр
        }

        GLuint shaderProgram = vec[0].shape->shader;
        glUseProgram(shaderProgram);
        GLint VP_Location = glGetUniformLocation(shaderProgram, "VP");
        Matrix4f VP = GameManager::render.PV;

        glUniformMatrix4fv(VP_Location, 1, GL_TRUE, &VP);
        glDrawElementsInstanced(GL_TRIANGLES, vec[0].shape->GetGeometry()->numIndices, GL_UNSIGNED_INT, 0, vec.size());
        glBindVertexArray(0);
    }


    class ConfigShapeBase {
    public:
        void attach(ConfigShapeBase &parent) {
            parent.component->AddChild(component);
        }

        glm::vec3 GetPosition() const { return component->GetPosition(); }
        glm::quat GetRotation() const { return component->GetRotation(); }
        glm::vec3 GetScale() const    { return component->GetScale(); }

        void SetPosition(const glm::vec3& position) { component->SetPosition(position); }
        void SetRotation(const glm::quat& rotation) { component->SetRotation(rotation); }
        void SetScale(const glm::vec3& scale)       { component->SetScale(scale); }

        void Move(const glm::vec3& offset)              { component->Move(offset); }
        void Move(glm::vec3 direction, float distance)  { component->Move(direction, distance); }
        void Rotate(const glm::quat& deltaRotation)     { component->Rotate(deltaRotation); }
        void RotateAround(const glm::vec3& axis, float angle) { component->RotateAround(axis, angle); }

        Component *component = nullptr;
    };

    template<typename C, typename S>
    class ConfigShape : public ConfigShapeBase {
    public:
        ConfigShape() {
            component = CreateComponent<C>();
            dynamic_cast<ComponentShape*>(component)->shape = ConfigShape<C, S>::shape;
        }

        static void Initialize() {
            shape = new S("", "shaders/white_fs.glsl", "shaders/instance_vs.glsl", "");
        }
        static void Deinitialization() {
            delete shape;
        }

        inline static S *shape = nullptr;
    };

    class ConfigMesh : public ConfigShapeBase {
    public:
        ConfigMesh(const std::string& Filename) {
            component = CreateComponent<ComponentMesh>();
            dynamic_cast<ComponentMesh*>(component)->mesh = ConfigMesh::mesh;
        }

        Mesh *mesh = nullptr;
    };

    using sprite_obj = ConfigShape<ComponentShape, Sprite>;
    using cube_obj   = ConfigShape<ComponentShape, Cube>;
    using sphere_obj = ConfigShape<ComponentShape, Sphere<sphere_quality>>;
    using mesh_obj   = ConfigMesh;

    class ConfigScene {
    public:
        // описание сцены
        ConfigScene();

        template<typename T>
        std::vector<T>& get_vector() {
            if constexpr (std::is_same_v<T, sprite_obj>) {
                return vector_sprites;
            } else if constexpr (std::is_same_v<T, cube_obj>) {
                return vector_cubes;
            } else if constexpr (std::is_same_v<T, sphere_obj>) {
                return vector_spheres;
            } else if constexpr (std::is_same_v<T, mesh_obj>) {
                return vector_meshes;
            } else {
                static_assert(sizeof(T) == 0, "Unsupported type");
            }
        }

        template<typename T>
        void add(T& obj) {
            auto& vec = get_vector<T>();
            vec.push_back(obj);
        }

        std::vector<sprite_obj> vector_sprites;
        std::vector<cube_obj>   vector_cubes;
        std::vector<sphere_obj> vector_spheres;
        std::vector<mesh_obj>   vector_meshes;
    };

    inline static std::string name;
    ConfigScene scene;
};

