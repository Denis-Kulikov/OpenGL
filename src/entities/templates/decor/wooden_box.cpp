#include <entities/templates/decor/wooden_box.hpp>
#include <managers/render/render.hpp> 
#include <object/component/component_physics.hpp>
#include <object/component/template/mesh.hpp>

WoodenBox::WoodenBox()
{
    ACTOR_SET_NAME;

    glm::vec3 scale(1.f);
    btScalar mass = 200.0f;
    btCollisionShape* colliderShape = new btBoxShape(btVector3(
        scale.x / 2, 
        scale.y / 2, 
        scale.z / 2
    ));
    
    RigidTransform* rigidBody = new RigidTransform(
        colliderShape, 
        mass, 
        scale
    );

    ComponentPhysics *body = CreateComponent<ComponentPhysics>(rigidBody);

    ComponentMesh *mesh = CreateComponent<ComponentMesh>(new Transform());
    body->AddChild(mesh);
    mesh->SetScale(scale);
    mesh->mesh = Mesh::Find("WoodenBox");

    rootComponent = body;
}

WoodenBox::~WoodenBox() {}

void WoodenBox::Initialize()
{
    originalName = "WoodenBox";
    auto shader = Shader::Create("cube", "shaders/sprite_fs.glsl", "shaders/mesh_vs.glsl");
    auto texture = Texture::Create("WoodenBox", "assets/img/box.jpg");
    MeshData& mDatal = RenderManager::primitives.cube;
    auto m = Mesh::Create("WoodenBox", mDatal, shader);
    m->m_Entries.begin()->Material.textureUnits.push_back({texture, TextureUnit::DIFFUSE});
}

ACTOR_BASE_CPP(WoodenBox);