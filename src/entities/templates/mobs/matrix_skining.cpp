#include <entities/templates/mobs/matrix_skining.hpp>
#include <object/component/template/skeletal_matrix_mesh.hpp>
#include <object/transform/transform.hpp>
#include <managers/global_state.hpp>

MatrixSkining::MatrixSkining()
{
    std::cout << name << std::endl;
    Transform *transform = new Transform();
    ComponentSkeletalMatrixMesh *mesh = CreateComponent<ComponentSkeletalMatrixMesh>(transform);
    mesh->SetSkeletalMesh(SkeletalMesh::Find("MatrixSkining"));
    mesh->animator->SetAnimationAny();

    rootComponent = mesh;
}

MatrixSkining::~MatrixSkining() {}

void MatrixSkining::Initialize()
{
    MatrixSkining::name = "MatrixSkining";
    std::cout << "Initialize: " << name << std::endl;
    // std::string path("assets/model/an_animated_cat.glb");
    std::string path("assets/model/female/female.glb");
    // std::string path("assets/model/2b.glb");
    // std::string path("assets/model/low_poly_fox_by_pixelmannen_animated.glb");
    // std::string path("assets/model/deer_demo_free_download.glb");

    auto shader_mesh = Shader::Create("skeletal_mesh", "shaders/sprite_fs.glsl", "shaders/skeletal_mesh_vs.glsl");
    SkeletalMeshData mDatal;
    GlobalState::MeshLoader->LoadMesh(path, mDatal);
    SkeletalMesh::Create("MatrixSkining", mDatal, shader_mesh);
}

std::string MatrixSkining::GetName() const {
    return MatrixSkining::name;
}
