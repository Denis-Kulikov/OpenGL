#include <entities/templates/mobs/female.hpp>
#include <object/component/template/mesh.hpp>
#include <object/transform/transform.hpp>
#include <managers/global.hpp>
#include <object/material/shader.hpp>

Female::Female()
{
    ACTOR_SET_NAME;

    Transform *transform = new Transform();
    ComponentMesh *mesh = CreateComponent<ComponentMesh>(transform);
    mesh->mesh = Mesh::Find("femele");

    rootComponent = mesh;
}

Female::~Female()
{}

void Female::Initialize()
{
    Female::originalName = "Female";
    std::cout << Female::originalName << std::endl;
    std::string path("assets/model/zhu/zhu_main.glb");
    // std::string path("assets/model/female/female.glb");
    auto shader = Shader::Create("mesh", "shaders/forward/mesh_pbr_fs.glsl", "shaders/forward/mesh_vs.glsl");
    MeshData mDatal;
    GlobalState::MeshLoader->LoadMesh(path, mDatal);
    Mesh::Create("femele", mDatal, shader);
}

ACTOR_BASE_CPP(Female);
