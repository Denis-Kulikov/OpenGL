#include <entities/templates/decor/stone_floor.hpp>
#include <managers/render/render.hpp> 
#include <object/component/component_physics.hpp>
#include <object/component/template/mesh.hpp>

std::string printVec3(const glm::vec3& v);
glm::vec3 quatToEuler(const glm::quat& q);

StoneFloor::StoneFloor()
{
    ACTOR_SET_NAME;

    btScalar mass = 0.0f;
    glm::vec3 scale(10, 10, 0.1f);
    btCollisionShape* colliderShape = new btBoxShape(btVector3(scale.x / 2, scale.y / 2, scale.z / 2));
    RigidTransform * rigidBody = new RigidTransform(colliderShape, mass, scale);
    ComponentPhysics *body = CreateComponent<ComponentPhysics>(rigidBody);
    body->SetRotation({-90, 0, 0});

    Transform *transform = new Transform();
    ComponentMesh *mesh = CreateComponent<ComponentMesh>(transform);
    body->AddChild(mesh);
    mesh->mesh = Mesh::Find("stone_floor");
    mesh->SetScale(scale);

    rootComponent = body;
}

StoneFloor::~StoneFloor() {}

void StoneFloor::Initialize()
{
    StoneFloor::originalName = "StoneFloor";
    // auto shader = Shader::Create("sprite", "shaders/sprite_fs.glsl", "shaders/mesh_vs.glsl");
    auto shader_mesh = Shader::Create("mesh", "shaders/forward/mesh_pbr_fs.glsl", "shaders/forward/mesh_vs.glsl");
    auto texture = Texture::Create("floor", "assets/img/floor.jpg");
    MeshData& mDatal = RenderManager::primitives.sprite;
    auto m = Mesh::Create("stone_floor", mDatal, shader_mesh);
    m->m_Entries.begin()->material.textureUnits.push_back({texture, TextureUnit::DIFFUSE});
}

ACTOR_BASE_CPP(StoneFloor);