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

    ComponentMesh *mesh = CreateComponent<ComponentMesh>("Mesh");
    mesh->SetMesh(Mesh::Find("House"));

    rootComponent = mesh;
}

House::~House() {}

void House::Initialize()
{
    originalName = "House";
    auto shader_mesh = Shader::Create("mesh", "shaders/forward/mesh_pbr_fs.glsl", "shaders/forward/mesh_vs.glsl");
    std::string path("assets/model/house/house.glb");
    // MeshData mDatal;
    // GlobalState::MeshLoader->LoadMesh(path, mDatal);
    auto m = Mesh::Create("House", path, shader_mesh);
}

ACTOR_BASE_CPP(House);