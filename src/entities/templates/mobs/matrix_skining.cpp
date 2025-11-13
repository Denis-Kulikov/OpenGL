#include <entities/templates/mobs/matrix_skining.hpp>
#include <object/component/template/skeletal_matrix_mesh.hpp>
#include <object/component/template/mesh.hpp>
#include <object/transform/transform.hpp>
#include <managers/global.hpp>
#include <object/material/shader.hpp>

MatrixSkining::MatrixSkining()
{
    ACTOR_SET_NAME;

    ComponentMesh *mesh = CreateComponent<ComponentMesh>("Mesh");
    mesh->SetMesh(SkeletalMesh::Find("MatrixSkining"));
    // ComponentSkeletalMatrixMesh *mesh = CreateComponent<ComponentSkeletalMatrixMesh>(transform);
    // mesh->SetSkeletalMesh(SkeletalMesh::Find("MatrixSkining"));
    // mesh->animator->SetAnimationAny();

    rootComponent = mesh;
}

MatrixSkining::~MatrixSkining() {}

void MatrixSkining::Initialize()
{
    MatrixSkining::originalName = "MatrixSkining";
    std::cout << MatrixSkining::originalName << std::endl;
    // std::string path("assets/model/2B_stay.dae");
    // std::string path("assets/model/female/female.glb");
    // std::string path("assets/model/zhu/zhu.glb");
    // std::string path("assets/model/zhu/zhu_main.glb");
    // std::string path("assets/model/low_poly_fox_by_pixelmannen_animated.glb");
    std::string path("assets/model/deer_demo_free_download.glb");
    // std::string path("assets/model/zhu2/zhu2.pmx");
    // std::string path("assets/model/zhu_yuan_from_zenless_zone_zero.glb");

    auto shader_mesh = Shader::Create("skeletal_mesh", "shaders/sprite_fs.glsl", "shaders/mesh_vs.glsl");
    // SkeletalMeshData mDatal;
    // GlobalState::MeshLoader->LoadMesh(path, mDatal);
    // GlobalState::MeshLoaderTiny->LoadMesh(path, mDatal);
    std::cout << "Load SkeletalMesh: " << std::endl;
    SkeletalMesh::Create("MatrixSkining", path, shader_mesh);
    std::cout << "End load SkeletalMesh: " << std::endl;
}

ACTOR_BASE_CPP(MatrixSkining);
