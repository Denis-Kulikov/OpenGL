#include <ui/visitor/component/title.hpp>
#include <ui/element/text.hpp>

#include <object/component/template/camera.hpp>
#include <object/component/template/mesh.hpp>
#include <object/component/template/point.hpp>
#include <object/component/template/skeletal_dq_mesh.hpp>
#include <object/component/template/skeletal_matrix_mesh.hpp>
#include <object/component/component_physics.hpp>


void UIVisitorComponentTitle::Visit(ComponentCamera& c) {
    UIText title("CameraTitle");
    title.SetText("Camera: {}", c.GetName().c_str());
    title.Draw();
}

void UIVisitorComponentTitle::Visit(ComponentMesh& c) {
    UIText title("MeshTitle");
    title.SetText("Mesh: {}", c.GetName().c_str());
    title.Draw();
}

void UIVisitorComponentTitle::Visit(ComponentPoint& c) {
    UIText title("PointTitle");
    title.SetText("Point: {}", c.GetName().c_str());
    title.Draw();
}

void UIVisitorComponentTitle::Visit(ComponentSkeletalDQMesh& c) {
    UIText title("SkeletalDQMeshTitle");
    title.SetText("SkeletalDQMesh: {}", c.GetName().c_str());
    title.Draw();
}

void UIVisitorComponentTitle::Visit(ComponentSkeletalMatrixMesh& c) {
    UIText title("SkeletalMatrixMeshTitle");
    title.SetText("SkeletalMatrixMesh: {}", c.GetName().c_str());
    title.Draw();
}


void UIVisitorComponentTitle::Visit(ComponentPhysics& c) {
    UIText title("PhysicsTitle");
    title.SetText("Physics: {}", c.GetName().c_str());
    title.Draw();
}
