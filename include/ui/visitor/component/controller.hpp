#pragma once
#include <ui/visitor/component.hpp>

class UIVisitorComponentController : public UIVisitorComponent {
public:
    // template
    void Visit(ComponentCamera& c) override;
    void Visit(ComponentMesh& c) override;
    void Visit(ComponentPoint& c) override;
    void Visit(ComponentSkeletalDQMesh& c) override;
    void Visit(ComponentSkeletalMatrixMesh& c) override;

    // base
    void Visit(ComponentPhysics& c) override;
};
