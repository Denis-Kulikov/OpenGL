#include <entities/templates/mobs/Female.hpp>
#include <object/component/template/mesh.hpp>
#include <object/component/template/skeletal_mesh.hpp>
#include <object/transform/transform.hpp>


Female::Female()
{
    std::cout << name << std::endl;
    Transform *transform = new Transform();
    ComponentSkeletalMesh *shape = CreateComponent<ComponentSkeletalMesh>(transform);
    shape->SetSkeletalMesh(mesh);
    shape->animator->SetAnimation("Take 001");
    rootComponent = shape;
    rootComponent->material = Material::Find("female");
}

Female::~Female()
{}

void Female::Initialize()
{
    Female::name = "Female";
    std::string path("assets/model/female/female.glb");
    mesh = new GeometrySkeletalMesh(path);

    auto shader_mesh = Shader::Create("mesh", "shaders/sprite_fs.glsl", "shaders/skeletal_mesh_vs.glsl");
    auto init_sprite = new Material::InitFunction([](Material& m) {
        std::vector<std::string> str = {"Projection", "View", "Model","textureSampler", "gBones"};
        for (auto s : str) {
            auto it = m.values.find(s);
            if (it != m.values.end()) {
                delete static_cast<glm::mat4*>(it->second.second);
                m.values.erase(it);
            }
        }
        m.values["Projection"] = {glGetUniformLocation(m.GetShader()->GetID(), "Projection"), new glm::mat4};
        m.values["View"] = {glGetUniformLocation(m.GetShader()->GetID(), "View"), new glm::mat4};
        // m.values["Model"] = {glGetUniformLocation(m.GetShader()->GetID(), "Model"), new glm::mat4};
        m.values["textureSampler"] = {glGetUniformLocation(m.GetShader()->GetID(), "textureSampler"), nullptr};
        m.values["gBones"] = {glGetUniformLocation(m.GetShader()->GetID(), "gBones"), nullptr};
    });
    auto apply_sprite = new Material::ApplyFunction([](const Material& m) {
        glUseProgram(m.GetShader()->GetID());
        glActiveTexture(GL_TEXTURE0);

        GLint texLoc = m.values.at("textureSampler").first;
        glUniform1i(texLoc, 0);

        glm::mat4* matPtr;
        GLint matLoc;

        matLoc = m.values.at("Projection").first;
        matPtr = static_cast<glm::mat4*>(m.values.at("Projection").second);
        glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(*matPtr));

        matLoc = m.values.at("View").first;
        matPtr = static_cast<glm::mat4*>(m.values.at("View").second);
        glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(*matPtr));

        // matLoc = m.values.at("Model").first;
        // matPtr = static_cast<glm::mat4*>(m.values.at("Model").second);
        // glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(*matPtr));
    });

    auto material_wooden_box = Material::Create("female", shader_mesh, init_sprite, apply_sprite);
    material_wooden_box->SetTexture(mesh->m_Textures);
    std::cout << mesh->m_Textures.size() << std::endl;
}

std::string Female::GetName() const {
    return Female::name;
}
