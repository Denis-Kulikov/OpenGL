#include <object/component/component_physics.hpp>


ComponentPhysics::ComponentPhysics(RigidTransform *transform)
    : Component(transform)
{}

void ComponentPhysics::UpdateInverse() { // virtual
    if (!fIsInit) {
        localTransform->UpdateMatrix();
        inverseTransform = glm::inverse(localTransform->GetMatrix());
    }
}

void ComponentPhysics::UpdateMatrix() { // virtual
    localTransform->UpdateMatrix();
}
