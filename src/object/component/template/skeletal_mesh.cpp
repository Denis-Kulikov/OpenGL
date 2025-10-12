#include <object/component/template/skeletal_mesh.hpp>

void ComponentSkeletalMesh::SetSkeletalMesh(SkeletalMesh *new_mesh) {
    if (new_mesh != nullptr) {
        mesh = new_mesh;
        animator = new Animator(mesh->GetSkeleton());
    }
}
