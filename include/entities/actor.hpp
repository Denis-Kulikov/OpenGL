#pragma once

#include <pugixml.hpp>

// #include "../object/objectTransform.hpp"
#include "../object/sprite.hpp"
// #include "../entities/components/component.hpp"

class Actor 
{
public:
    Actor(const std::string &_name, const objectTransform &_trans);
    Actor(const std::string &path);

    std::string name;
    objectTransform trans;

    bool loadActor(const std::string &path);
    // bool loadComponents(const string &path);
    // void addComponent();

    // std::vector<Component> components; 
    // std::vector<overallAnimation> overallAnimation; 
};
