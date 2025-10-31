#include <entities/templates/decor/grass.hpp>
#include <managers/render_manager.hpp> 
#include <object/material/material.hpp>
#include <object/material/shader.hpp>

Grass::Grass()
{
    std::cout << name << std::endl;
    Transform *transform = new Transform();
    ComponentShape *shape = CreateComponent<ComponentShape>(transform);
    shape->shape = RenderManager::primitives.sprite;
    shape->material = Material::Find("grass");

    // TransformDQ *transform2 = new TransformDQ();
    // ComponentShape *shape2 = CreateComponent<ComponentShape>(transform2);
    // shape->AddChild(shape2);
    // shape2->shape = RenderManager::primitives.sprite;
    // shape2->material = Material::Find("grass");
    // shape2->SetPosition({0, 1, 0});

    rootComponent = shape;
}

Grass::~Grass() {}

void Grass::Initialize()
{
    Grass::name = "Grass";
    
    auto shader_cube = Shader::Create("sprite", "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl");
    auto texture_skybox = Texture::Create("grass", "assets/img/grass.png");
    auto material_skybox = Material::Create("grass", shader_cube, init_sprite, apply_sprite);
    material_skybox->PushTexture(texture_skybox);
}

std::string Grass::GetName() const {
    return Grass::name;
}
