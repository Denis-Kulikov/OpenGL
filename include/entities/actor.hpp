#pragma once
#include "../object/objectTransform.hpp"
#include "../object/sprite.hpp"
#include "../object/component.hpp"

class Actor 
{
public:
    Actor(const std::string &_name, const objectTransform &_trans);

    std::string name;
    objectTransform trans;

    std::map<sprite, std::string> sprites; 
    std::vector<Component> *components; 
};
