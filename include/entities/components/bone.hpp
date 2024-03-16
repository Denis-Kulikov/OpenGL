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

    void printBones(int lvl);
    size_t addChildBone(pugi::xml_node &node, std::string _name, Bone *_parent);
    size_t parseBone(pugi::xml_node &node, Bone *_parent);
    size_t createSkelet(const std::string &_path, const std::string &_name);


    std::string name;
    Bone *parent = nullptr;
    std::vector<Bone*> children;
    std::map<std::string, Animation> Animations;
};
    // objectTransform _anchorPoint


