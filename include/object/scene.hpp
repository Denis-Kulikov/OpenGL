#pragma once
#include "sprite.hpp"
#include "sphere.hpp"
#include "circle.hpp"
#include "line.hpp"
#include "cube_bone.hpp"
#include "../entities/components/component.hpp"

class Scene 
{
public:
    Scene(std::string _name) : name(_name) {};
    ~Scene() {};

    void pushObject(Component _component) { component.push_back(_component); };

    void deleteObject(Component _component) {};

    std::vector<Component>::iterator getIterator() { return component.begin(); }

    std::string name;
    std::vector<Component> component;
};
