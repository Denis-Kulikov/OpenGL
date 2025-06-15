#include <entities/templates/decor/wooden_box.hpp>
#include <managers/render_manager.hpp> 
#include <object/component/template/component_physics.hpp>

WoodenBox::WoodenBox()
{
    std::cout << name << std::endl;
    Transform *transform1 = new Transform();
    ComponentShape *shape1 = CreateComponent<ComponentShape>(transform1);
    shape1->shape = RenderManager::primitives.cube;
    shape1->SetPosition(glm::vec3(0.0, 0, 0));
    // shape1->SetRotation(glm::vec3(0.0, 45, 0));
    // shape1->SetScale(glm::vec3(0.5));
    shape1->material = Material::Find("WoodenBox");

    // Transform *transform2 = new Transform();
    // ComponentShape *shape2 = CreateComponent<ComponentShape>(transform2);
    // shape2->shape = RenderManager::primitives.cube;
    // shape1->AddChild(shape2);
    // shape2->SetPosition(glm::vec3(1.0, .75, 0.0));
    // shape2->SetRotation(glm::vec3(0.0, 0, 0));
    // // shape2->SetScale(glm::vec3(0.5));
    // // shape2->SetScale(glm::vec3(1.0, 1, 1));
    // shape2->SetScale(glm::vec3(1));
    // shape2->material = Material::Find("WoodenBox");

    // Transform *transform3 = new Transform();
    // ComponentShape *shape3 = CreateComponent<ComponentShape>(transform3);
    // shape3->shape = RenderManager::primitives.cube;
    // shape2->AddChild(shape3);
    // shape3->SetPosition(glm::vec3(0.0, 1.75, 0.0));
    // // shape3->SetRotation(glm::vec3(0, 45, 0));
    // shape3->SetScale(glm::vec3(2.0, 1, 1.0));
    // shape3->material = Material::Find("WoodenBox");


    rootComponent = shape1;
}

WoodenBox::~WoodenBox() {}

void WoodenBox::Initialize()
{
    WoodenBox::name = "WoodenBox";
    auto shader_cube = Shader::Create("cube", "shaders/cube_fs.glsl", "shaders/cube_vs.glsl");
    auto texture_wooden_box = Texture::Create("wooden_box", "img/box.jpg");
    
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
    });
    auto apply_sprite = new Material::ApplyFunction([](const Material& m) {
        glActiveTexture(GL_TEXTURE0);

        GLint texLoc = m.values.at("textureSampler").first;
        glUniform1i(texLoc, 0);
        GLint matLoc = m.values.at("gWorld").first;
        glm::mat4* matPtr = static_cast<glm::mat4*>(m.values.at("gWorld").second);
        glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(*matPtr));
    });

    auto material_wooden_box = Material::Create("WoodenBox", shader_cube, init_sprite, apply_sprite);
    material_wooden_box->PushTexture(texture_wooden_box);
}

std::string WoodenBox::GetName() const {
    return WoodenBox::name;
}
