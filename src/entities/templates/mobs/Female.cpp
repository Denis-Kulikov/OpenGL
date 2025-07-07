#include <entities/templates/mobs/Female.hpp>
#include <object/component/template/mesh.hpp>
#include <object/component/template/skeletal_mesh.hpp>
#include <object/transform/transform.hpp>


Female::Female()
{
    std::cout << name << std::endl;
    Transform *transform = new Transform();
    ComponentSkeletalMesh *shape = CreateComponent<ComponentSkeletalMesh>(transform);
    shape->SetSkeletalMesh(GeometrySkeletalMesh::Find("Female"));
    // shape->animator->SetAnimation("Take 001");
    // shape->animator->SetAnimation("Object_0");
    shape->animator->SetAnimationAny();
    shape->material = Material::Find("female");

    rootComponent = shape;
}

Female::~Female()
{}

void Female::Initialize()
{
    Female::name = "Female";
    // std::string path("assets/model/female/female.glb");
    // std::string path("assets/model/horse_trot/scene.gltf");
    std::string path("assets/model/my_model_no_rotated.dae");
    auto mesh = GeometrySkeletalMesh::Create("Female", path);

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
        
        auto id = m.GetShader()->GetID();

        m.values["Projection"]  = {glGetUniformLocation(id, "Projection"), new glm::mat4};
        m.values["View"]        = {glGetUniformLocation(id, "View"), new glm::mat4};
        m.values["Model"]       = {glGetUniformLocation(id, "Model"), new glm::mat4};
        
        m.values["gDQ"]  = {glGetUniformLocation(id, "gDQ"), nullptr};
        // m.values["gBoneRotations"]  = {glGetUniformLocation(id, "gBoneRotations"), nullptr};
        // m.values["gBoneTranslations"] = {glGetUniformLocation(id, "gBoneTranslations"), nullptr};
        
        m.values["textureSampler"] = {glGetUniformLocation(id, "textureSampler"), nullptr};
        m.values["gBones"]         = {glGetUniformLocation(id, "gBones"), nullptr};
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

        matLoc = m.values.at("Model").first;
        matPtr = static_cast<glm::mat4*>(m.values.at("Model").second);
        glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(*matPtr));
    });

    auto material_wooden_box = Material::Create("female", shader_mesh, init_sprite, apply_sprite);
    material_wooden_box->SetTexture(mesh->m_Textures);
}

std::string Female::GetName() const {
    return Female::name;
}
