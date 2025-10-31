#include <entities/templates/decor/skybox.hpp>
#include <managers/render/render.hpp> 
#include <object/material/material.hpp>
#include <object/material/shader.hpp>
#include <object/material/texture.hpp>
#include <object/material/texture_unit.hpp>

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
    std::cout << Skybox::originalName << std::endl;
    auto shader = Shader::Create("cube", "shaders/sprite_fs.glsl", "shaders/mesh_vs.glsl");
    auto texture = Texture::Create("skybox", "assets/img/skybox.png");
    MeshData& mDatal = RenderManager::primitives.cube;
    auto m = Mesh::Create("skybox", mDatal, shader);
    TextureUnit unit(texture, TextureUnit::ALBEDO);
    m->m_Entries.begin()->material->PushTextureUnits(m->material->textureUnits.size(), &unit, shader);
}

ACTOR_BASE_CPP(Skybox);
