#include <entities/templates/mobs/dual_quat_skining.hpp>
#include <object/component/template/skeletal_dq_mesh.hpp>
#include <object/transform/transform.hpp>


DualQuatSkining::DualQuatSkining()
{
    std::cout << name << std::endl;
    // Transform *transform = new Transform();
    // ComponentSkeletalDQMesh *shape = CreateComponent<ComponentSkeletalDQMesh>(transform);
    // shape->SetSkeletalMesh(GeometrySkeletalMesh::Find("DualQuatSkining"));
    // shape->animator->SetAnimationAny();
    // shape->material = Material::Find("meshDQ");

    // rootComponent = shape;
}

DualQuatSkining::~DualQuatSkining()
{}

void DualQuatSkining::Initialize()
{
    DualQuatSkining::name = "DualQuatSkining";
    // // std::string path("assets/model/my_model_no_rotated.dae");
    // std::string path("assets/model/an_animated_cat.glb");
    // // std::string path("assets/model/deer_demo_free_download.glb");
    // // std::string path("assets/model/female/female.glb");
    // // std::string path("assets/model/my_model_no_rotated_move.dae");
    // // std::string path("assets/model/my_model_rotated.dae");
    // auto mesh = GeometrySkeletalMesh::Create("DualQuatSkining", path);
    // auto shader_mesh = Shader::Create("meshDQ", "shaders/sprite_fs.glsl", "shaders/skeletal_mesh_dqs_vs.glsl");
}

std::string DualQuatSkining::GetName() const {
    return DualQuatSkining::name;
}
