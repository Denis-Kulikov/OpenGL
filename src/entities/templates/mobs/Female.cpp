#include <entities/templates/mobs/female.hpp>
#include <object/component/template/mesh.hpp>
#include <object/transform/transform.hpp>
#include <managers/global.hpp>


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
    std::string path("assets/model/female/female.glb");
    auto shader_mesh = Shader::Create("mesh", "shaders/sprite_fs.glsl", "shaders/mesh_vs.glsl");
    MeshData mDatal;
    GlobalState::MeshLoader->LoadMesh(path, mDatal);
    Mesh::Create("femele", mDatal, shader_mesh);
}

ACTOR_BASE_CPP(Female);
