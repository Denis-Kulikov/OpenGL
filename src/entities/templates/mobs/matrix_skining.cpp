#include <entities/templates/mobs/matrix_skining.hpp>
#include <object/component/template/mesh.hpp>
#include <object/component/template/skeletal_matrix_mesh.hpp>
#include <object/transform/transform.hpp>


MatrixSkining::MatrixSkining()
{
    std::cout << name << std::endl;
    Transform *transform = new Transform();
    ComponentSkeletalMatrixMesh *shape = CreateComponent<ComponentSkeletalMatrixMesh>(transform);
    shape->SetSkeletalMesh(GeometrySkeletalMesh::Find("MatrixSkining"));
    shape->animator->SetAnimationAny();
    shape->material = Material::Find("MatrixSkining");

    rootComponent = shape;
}

MatrixSkining::~MatrixSkining()
{}

void MatrixSkining::Initialize()
{
    MatrixSkining::name = "MatrixSkining";
    std::string path("assets/model/female/female.glb");
    // std::string path("assets/model/rotate_test.glb");
    // std::string path("assets/model/an_animated_cat.glb");
    // std::string path("assets/model/deer_demo_free_download.glb");
    // std::string path("assets/model/my_model_no_rotated.dae");
    auto mesh = GeometrySkeletalMesh::Create("MatrixSkining", path);

    auto shader_mesh = Shader::Create("skeletal_mesh", "shaders/sprite_fs.glsl", "shaders/skeletal_mesh_vs.glsl");
    auto init_sprite = new Material::InitFunction([](Material& m) {
        auto id = m.GetShader()->GetID();

        m.values["Projection"]  = {glGetUniformLocation(id, "Projection"), new glm::mat4};
        m.values["View"]        = {glGetUniformLocation(id, "View"), new glm::mat4};
        m.values["Model"]       = {glGetUniformLocation(id, "Model"), new glm::mat4};
        
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

    auto material_wooden_box = Material::Create("MatrixSkining", shader_mesh, init_sprite, apply_sprite);
    material_wooden_box->SetTexture(mesh->m_Textures);
}

std::string MatrixSkining::GetName() const {
    return MatrixSkining::name;
}
