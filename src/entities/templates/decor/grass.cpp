#include <entities/templates/decor/grass.hpp>
#include <managers/render_manager.hpp> 

Grass::Grass()
{
    std::cout << name << std::endl;
    Transform *transform = new Transform();
    ComponentShape *shape = CreateComponent<ComponentShape>(transform);
    shape->shape = sprite;
    rootComponent = shape;
    rootComponent->material = Material::Find("grass");
}

Grass::~Grass() {}

void Grass::Initialize()
{
    Grass::name = "skybox";
    sprite = RenderManager::primitives.sprite;
    
    auto shader_cube = Shader::Create("cube", "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl");
    auto texture_skybox = Texture::Create("grass", "img/grass.png");
    
    auto init_sprite = new Material::InitFunction([](Material& m) {
        std::vector<std::string> str = {"gWorld", "textureSampler"};
        for (auto s : str) {
            auto it = m.values.find(s);
            if (it != m.values.end()) {
                delete static_cast<glm::mat4*>(it->second.second);
                m.values.erase(it);
            }
        }
        m.values["gWorld"] = {glGetUniformLocation(m.GetShader()->GetID(), "gWorld"), new glm::mat4};
        m.values["textureSampler"] = {glGetUniformLocation(m.GetShader()->GetID(), "textureSampler"), nullptr};

        for (const auto& it : m.values) {
            if (it.second.first < 0) {
                std::cout << "init: " << it.first << " " << it.second.first << std::endl;
            }
            
        }
    });
    auto apply_sprite = new Material::ApplyFunction([](const Material& m) {
        glUseProgram(m.GetShader()->GetID());
        glActiveTexture(GL_TEXTURE0);

        GLint texLoc = m.values.at("textureSampler").first;
        glUniform1i(texLoc, 0);
        GLint matLoc = m.values.at("gWorld").first;
        glm::mat4* matPtr = static_cast<glm::mat4*>(m.values.at("gWorld").second);
        glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(*matPtr));

    });

    auto material_skybox = Material::Create("grass", shader_cube, init_sprite, apply_sprite);
    material_skybox->PushTexture(texture_skybox);
}

std::string Grass::GetName() const {
    return Grass::name;
}
