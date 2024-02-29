#pragma once
#include "../object/objectTransform.hpp"

class Component
{
public:
    Component(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> &_anchorPoint, const component *_parent, const int _zIndex, count frameCount);

    bool addChildComponent();

    std::string name;
    std::string spriteName;
    objectTransform trans;
    objectTransform anchorPoint;

    component *parent = nullptr;
    std::vector<component*> children;

    int zIndex;
};
