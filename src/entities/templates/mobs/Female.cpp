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
    // std::string path("assets/model/zhu/zhu.dae");
    // std::string path("assets/model/zhu_yuan_from_zenless_zone_zero.glb");
    // std::string path("assets/model/zzz-zhu-yuan/source/ZZZ - Zhu/ZZZ - Zhu.glb");
    // std::string path("assets/model/zhu/zhu.pmx");
    // std::string path("assets/model/zhu_yuan_from_zenless_zone_zero.glb");
    std::string path("assets/model/female/female.glb");
    // auto shader_mesh = Shader::Create("mesh", "shaders/with_lighting/mesh_fs.glsl", "shaders/with_lighting/mesh_vs.glsl");
    auto shader_mesh = Shader::Create("mesh", "shaders/forward/mesh_pbr_fs.glsl", "shaders/forward/mesh_vs.glsl");
    MeshData mDatal;
    GlobalState::MeshLoader->LoadMesh(path, mDatal);
    Mesh::Create("femele", mDatal, shader_mesh);
}

ACTOR_BASE_CPP(Female);
