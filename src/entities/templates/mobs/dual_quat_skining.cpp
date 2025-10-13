#include <entities/templates/mobs/dual_quat_skining.hpp>
#include <object/component/template/skeletal_dq_mesh.hpp>
#include <object/transform/transform.hpp>
#include <managers/global_state.hpp>


DualQuatSkining::DualQuatSkining()
{
    std::cout << name << std::endl;
    Transform *transform = new Transform();
    ComponentSkeletalDQMesh *mesh = CreateComponent<ComponentSkeletalDQMesh>(transform);
    mesh->SetSkeletalMesh(SkeletalMesh::Find("DualQuatSkining"));
    mesh->animator->SetAnimationAny();

    rootComponent = mesh;
}

DualQuatSkining::~DualQuatSkining()
{}

void DualQuatSkining::Initialize()
{
    DualQuatSkining::name = "DualQuatSkining";
    // std::string path("assets/model/an_animated_cat.glb");
    std::string path("assets/model/female/female.glb");
    auto shader_mesh = Shader::Create("skeletal_mesh_dq", "shaders/sprite_fs.glsl", "shaders/skeletal_mesh_dqs_vs.glsl");
    SkeletalMeshData mDatal;
    GlobalState::MeshLoader->LoadMesh(path, mDatal);
    SkeletalMesh::Create("DualQuatSkining", mDatal, shader_mesh);
}

std::string DualQuatSkining::GetName() const {
    return DualQuatSkining::name;
}
