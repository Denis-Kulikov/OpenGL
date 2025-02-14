#pragma once
#include <btBulletDynamicsCommon.h>
#include <lib-project/lib.hpp>
#include <game/gameManager.hpp>
#include <mesh/mesh.hpp>
#include "../component.hpp"

class ComponentPoint : public Component {
public:
    ComponentPoint(Transform *transform)
        : Component(transform) {}
    ComponentPoint(RigidTransform *transform)
        : Component(transform) {}

    void Render() const override {}
};
