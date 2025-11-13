#include <entities/templates/mobs/tmodel.hpp>
#include <object/component/template/mesh.hpp>
#include <object/transform/transform.hpp>
#include <managers/global.hpp>
#include <object/material/shader.hpp>

TModel::TModel()
{
    ACTOR_SET_NAME;

    ComponentMesh *mesh = CreateComponent<ComponentMesh>("Mesh");
    mesh->SetMesh(Mesh::Find("femele"));

    rootComponent = mesh;
}

TModel::~TModel()
{}

void TModel::Initialize()
{
    TModel::originalName = "TModel";
    std::cout << TModel::originalName << std::endl;
    // std::string path("assets/model/fence/fence.glb");
    // std::string path("assets/model/concrete_fence_low-poly/scene.gltf");
    std::string path("assets/model/zhu/zhu_main.glb");
    // std::string path("assets/model/TModel/TModel.glb");
    auto shader = Shader::Create("mesh", "shaders/forward/mesh_pbr_fs.glsl", "shaders/forward/mesh_vs.glsl");
    // MeshData mDatal;
    // GlobalState::MeshLoader->LoadMesh(path, mDatal);
    Mesh::Create("femele", path, shader);
}

ACTOR_BASE_CPP(TModel);
