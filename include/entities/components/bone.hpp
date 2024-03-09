#pragma once

#include <pugixml.hpp>
#include "animation.hpp"
// #include "component.hpp"

class Bone
{
public:
    Bone(const std::string &_name, Bone *_parent) : name(_name), parent(_parent) {};
    Bone(const std::string &_name) : name(_name) {};
    Bone() {};

    static const char BONE[];
    static const char NAME[];

    void addChildBone(pugi::xml_node &node, std::string _name, Bone *_parent);
    void parseBone(pugi::xml_node &node, Bone *_parent);
    void createSkelet(const std::string &_path, const std::string &_name);


    std::string name;
    Bone *parent = nullptr;
    std::vector<Bone*> children;
    Animation animation;
    // std::vector<Components> animation;
};
    // objectTransform _anchorPoint


