#include <object/component/template/component_physics.hpp>


ComponentPhysics::ComponentPhysics(RigidTransform *transform)
    : Component(transform)
{}

void ComponentPhysics::UpdateInverse() {
    if (!fIsInit) {
        localTransform->UpdateMatrix();
        inverseTransform = glm::inverse(localTransform->GetMatrix());
        fIsInit = true;
    }
}

void ComponentPhysics::UpdateMatrix() {
    localTransform->UpdateMatrix();
}
