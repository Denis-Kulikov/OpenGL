#pragma once

// class Component; // virtual
class ComponentCamera;
class ComponentMesh;
class ComponentPoint;
class ComponentSkeletalDQMesh;
class ComponentSkeletalMatrixMesh;
// class ComponentSkeletalMesh; // virtual

class ComponentPhysics;


class UIVisitorComponent {
public:
    // template
    virtual void Visit(ComponentCamera& c) = 0;
    virtual void Visit(ComponentMesh& c) = 0;
    virtual void Visit(ComponentPoint& c) = 0;
    virtual void Visit(ComponentSkeletalDQMesh& c) = 0;
    virtual void Visit(ComponentSkeletalMatrixMesh& c) = 0;

    // base
    virtual void Visit(ComponentPhysics& c) = 0;
};
