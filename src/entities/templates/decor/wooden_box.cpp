#include <entities/templates/decor/wooden_box.hpp>
#include <managers/render_manager.hpp> 

WoodenBox::WoodenBox()
{
    Transform *transform1 = new Transform();
    ComponentShape *shape1 = CreateComponent<ComponentShape>(transform1);
    transform1->SetScale(glm::vec3(1.0));
    shape1->shape = cube;
    shape1->material = Material::Find("WoodenBox");

    Transform *transform2 = new Transform();
    ComponentShape *shape2 = CreateComponent<ComponentShape>(transform2);
    shape2->shape = cube;
    transform2->SetScale(glm::vec3(0.5, 3, 0.5));
    shape2->material = Material::Find("WoodenBox");

    shape1->AddChild(shape2);

    rootComponent = shape1;
}

WoodenBox::~WoodenBox() {}

void WoodenBox::Initialize()
{
    WoodenBox::name = "WoodenBox";
    cube = RenderManager::primitives.cube;
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
        glUseProgram(m.GetShader()->GetID());
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
