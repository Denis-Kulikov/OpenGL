#include <entities/templates/mobs/Female.hpp>
#include <object/component/template/shape.hpp>
#include <object/component/template/mesh.hpp>
#include <object/transform/transform.hpp>


Female::Female()
{
    std::cout << name << std::endl;
    Transform *transform = new Transform();
    ComponentShape *shape = CreateComponent<ComponentShape>(transform);
    shape->shape = mesh;
    rootComponent = shape;
    rootComponent->material = Material::Find("female");
}

Female::~Female()
{}

void Female::Initialize()
{
    Female::name = "Female";
    std::string path("assets/model/female/female.glb");
    mesh = new GeometryMesh(path);

    auto shader_cube = Shader::Create("sprite", "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl");
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

    auto material_wooden_box = Material::Create("female", shader_cube, init_sprite, apply_sprite);
    material_wooden_box->SetTexture(mesh->m_Textures);
    std::cout << mesh->m_Textures.size() << std::endl;
}

std::string Female::GetName() const {
    return Female::name;
}
