#pragma once
#include "../../object/objectTransform.hpp"

class Component
{
public:
    Component(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> &_anchorPoint, const Component *_parent, const int _zIndex, const size_t _frameCount);

    bool addChildComponent(const std::string &_name, const objectTransform &_trans, const objectTransform &_anchorPoint, const Component *_parent, const int _zIndex, const size_t _frameCount);

    std::string name;
    objectTransform trans;
    objectTransform anchorPoint;

    Component *parent = nullptr;
    std::vector<Component*> children;

    int zIndex;
    size_t frameCount;
};
