#pragma once
#include "component.hpp"

class ComponentPhysics : public Component {
public:
    ComponentPhysics(RigidTransform *transform);

    virtual void UpdateInverse() override;
    virtual void UpdateMatrix() override;
    void Render() const override {};

    bool fIsInit = 0;
};


// ComponentPhysics* CreateComponent(RigidTransform *transform) {
//     return new ComponentPhysics(transform);
// }
