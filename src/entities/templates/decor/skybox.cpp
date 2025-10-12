#include <entities/templates/decor/skybox.hpp>
#include <managers/render_manager.hpp> 

Skybox::Skybox()
{
    std::cout << name << std::endl;
    // Transform *transform = new Transform();
    // ComponentMesh *mesh = CreateComponent<ComponentMesh>(transform);
    // mesh->mesh = RenderManager::primitives.cube;
    // mesh->material = Material::Find("skybox");
    // rootComponent = mesh;
}

Skybox::~Skybox() {}

void Skybox::Initialize()
{
    Skybox::name = "skybox";
    auto shader_cube = Shader::Create("cube", "shaders/cube_fs.glsl", "shaders/cube_vs.glsl");
    auto texture_skybox = Texture::Create("skybox", "assets/img/skybox.png");
    // auto material_skybox = Material::Create("skybox", shader_cube);
    // material_skybox->PushTexture(texture_skybox);
}

std::string Skybox::GetName() const {
    return Skybox::name;
}
