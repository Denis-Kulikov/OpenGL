#include <entities/templates/mobs/dual_quat_skining.hpp>
#include <object/component/template/skeletal_dq_mesh.hpp>
#include <object/transform/transform.hpp>
#include <managers/global.hpp>


DualQuatSkining::DualQuatSkining()
{
    ACTOR_SET_NAME;

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
    DualQuatSkining::originalName = "DualQuatSkining";
    // std::string path("assets/model/an_animated_cat.glb");
    std::string path("assets/model/female/female.glb");
    auto shader_mesh = Shader::Create("skeletal_mesh_dq", "shaders/sprite_fs.glsl", "shaders/skeletal_mesh_dqs_vs.glsl");
    SkeletalMeshData mDatal;
    GlobalState::MeshLoader->LoadMesh(path, mDatal);
    SkeletalMesh::Create("DualQuatSkining", mDatal, shader_mesh);
}

ACTOR_BASE_CPP(DualQuatSkining);
