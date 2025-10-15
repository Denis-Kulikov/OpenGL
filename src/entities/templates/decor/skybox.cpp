#include <entities/templates/decor/skybox.hpp>
#include <managers/render/render.hpp> 

Skybox::Skybox()
{
    ACTOR_SET_NAME;

    Transform *transform = new Transform();
    ComponentMesh *mesh = CreateComponent<ComponentMesh>(transform);
    mesh->mesh = Mesh::Find("skybox");
    rootComponent = mesh;
}

Skybox::~Skybox() {}

void Skybox::Initialize()
{
    Skybox::originalName = "skybox";
    auto shader = Shader::Create("cube", "shaders/sprite_fs.glsl", "shaders/mesh_vs.glsl");
    auto texture = Texture::Create("skybox", "assets/img/skybox.png");
    MeshData& mDatal = RenderManager::primitives.cube;
    auto m = Mesh::Create("skybox", mDatal, shader);
    m->m_Entries.begin()->Material.textureUnits.push_back({texture, TextureUnit::DIFFUSE});
}

ACTOR_BASE_CPP(Skybox);
