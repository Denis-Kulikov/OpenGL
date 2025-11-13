#include <entities/templates/decor/wooden_box.hpp>
#include <managers/render/render.hpp> 
#include <object/component/component_physics.hpp>
#include <object/component/template/mesh.hpp>
#include <object/material/material.hpp>
#include <object/material/shader.hpp>
#include <object/material/texture.hpp>
#include <object/material/texture_unit.hpp>

WoodenBox::WoodenBox()
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

    ComponentMesh *mesh = CreateComponent<ComponentMesh>("Mesh");
    // body->AddChild(mesh);
    // mesh->SetScale(scale * 5.f);
    mesh->SetMesh(Mesh::Find("WoodenBox"));

    rootComponent = mesh;
    // rootComponent = body;
}

WoodenBox::~WoodenBox() {}

void WoodenBox::Initialize()
{
    originalName = "WoodenBox";
    auto shader_mesh = Shader::Create("mesh", "shaders/forward/mesh_pbr_fs.glsl", "shaders/forward/mesh_vs.glsl");
    auto texture = Texture::Create("WoodenBox", "assets/img/box.jpg");
    MeshData& mDatal = RenderManager::primitives.cube;
    auto m = Mesh::Create("WoodenBox", mDatal, shader_mesh);
    TextureUnit unit(texture, TextureUnit::ALBEDO);
    m->m_Entries.begin()->material->PushTextureUnits(m->material->textureUnits.size(), &unit, shader_mesh);
}

ACTOR_BASE_CPP(WoodenBox);