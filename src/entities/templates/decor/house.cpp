#include <entities/templates/decor/house.hpp>
#include <managers/render/render.hpp> 
#include <object/component/template/mesh.hpp>
#include <managers/global.hpp>
#include <object/material/material.hpp>
#include <object/material/shader.hpp>
#include <object/material/texture.hpp>
#include <object/material/texture_unit.hpp>

House::House()
{
    ACTOR_SET_NAME;

    // glm::vec3 scale(1.f);
    // btScalar mass = 200.0f;
    // btCollisionShape* colliderShape = new btBoxShape(btVector3(
    //     scale.x / 2, 
    //     scale.y / 2, 
    //     scale.z / 2
    // ));
    
    // RigidTransform* rigidBody = new RigidTransform(
    //     colliderShape, 
    //     mass, 
    //     scale
    // );

    // ComponentPhysics *body = CreateComponent<ComponentPhysics>(rigidBody);

    ComponentMesh *mesh = CreateComponent<ComponentMesh>(new Transform());
    // body->AddChild(mesh);
    // mesh->SetScale(scale * 5.f);
    mesh->mesh = Mesh::Find("House");

    rootComponent = mesh;
    // rootComponent = body;
}

House::~House() {}

void House::Initialize()
{
    originalName = "House";
    auto shader_mesh = Shader::Create("mesh", "shaders/forward/mesh_pbr_fs.glsl", "shaders/forward/mesh_vs.glsl");
    std::string path("assets/model/house/house.glb");
    MeshData mDatal;
    GlobalState::MeshLoader->LoadMesh(path, mDatal);
    auto m = Mesh::Create("House", mDatal, shader_mesh);
}

ACTOR_BASE_CPP(House);