#include <entities/templates/mobs/dual_quat_skining.hpp>
#include <object/component/template/mesh.hpp>
#include <object/component/template/skeletal_dq_mesh.hpp>
#include <object/transform/transform.hpp>


DualQuatSkining::DualQuatSkining()
{
    std::cout << name << std::endl;
    Transform *transform = new Transform();
    ComponentSkeletalDQMesh *shape = CreateComponent<ComponentSkeletalDQMesh>(transform);
    shape->SetSkeletalMesh(GeometrySkeletalMesh::Find("DualQuatSkining"));
    shape->animator->SetAnimationAny();
    shape->material = Material::Find("meshDQ");

    rootComponent = shape;
}

DualQuatSkining::~DualQuatSkining()
{}

void DualQuatSkining::Initialize()
{
    DualQuatSkining::name = "DualQuatSkining";
    // std::string path("assets/model/my_model_no_rotated.dae");
    std::string path("assets/model/an_animated_cat.glb");
    // std::string path("assets/model/female/female.glb");
    // std::string path("assets/model/my_model_no_rotated_move.dae");
    // std::string path("assets/model/my_model_rotated.dae");
    auto mesh = GeometrySkeletalMesh::Create("DualQuatSkining", path);

    auto shader_mesh = Shader::Create("meshDQ", "shaders/sprite_fs.glsl", "shaders/skeletal_mesh_dqs_vs.glsl");
    auto init_sprite = new Material::InitFunction([](Material& m) {
        
        auto id = m.GetShader()->GetID();

        m.values["Projection"]  = {glGetUniformLocation(id, "Projection"), new glm::mat4};
        m.values["View"]        = {glGetUniformLocation(id, "View"), new glm::mat4};
        m.values["Model"]       = {glGetUniformLocation(id, "Model"), new glm::mat4};
        m.values["gDQ"]         = {glGetUniformLocation(id, "gDQ"), nullptr};
        
        m.values["textureSampler"] = {glGetUniformLocation(id, "textureSampler"), nullptr};
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

    auto material_wooden_box = Material::Create("meshDQ", shader_mesh, init_sprite, apply_sprite);
    material_wooden_box->SetTexture(mesh->m_Textures);
}

std::string DualQuatSkining::GetName() const {
    return DualQuatSkining::name;
}
