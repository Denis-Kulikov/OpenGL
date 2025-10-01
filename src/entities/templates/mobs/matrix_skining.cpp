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

MatrixSkining::~MatrixSkining() {}

void MatrixSkining::Initialize()
{
    MatrixSkining::name = "MatrixSkining";
    //std::string path("assets/model/female/female.glb");
    // std::string path("assets/model/rotate_test.glb");
     std::string path("assets/model/an_animated_cat.glb");
    // std::string path("assets/model/deer_demo_free_download.glb");
    // std::string path("assets/model/my_model_no_rotated.dae");

    auto mesh = GeometrySkeletalMesh::Create("MatrixSkining", path);
    auto shader_mesh = Shader::Create("skeletal_mesh", "shaders/sprite_fs.glsl", "shaders/skeletal_mesh_vs.glsl");
    auto material_wooden_box = Material::Create("MatrixSkining", shader_mesh);
    material_wooden_box->SetTexture(mesh->m_Textures);
}

std::string MatrixSkining::GetName() const {
    return MatrixSkining::name;
}
