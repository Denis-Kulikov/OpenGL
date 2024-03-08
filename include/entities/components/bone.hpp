#pragma once

#include <pugixml.hpp>
#include "components.hpp"

class Bone
{
public:
    Bone(const std::string &_name, Bone *_parent) : name(_name), parent(_parent) {};
    Bone(const std::string &_name) : name(_name) {};
    Bone() {};

    static const char BONE[];
    static const char NAME[];

    void addChildBone(pugi::xml_node &node, std::string _name, Bone *_parent, std::map<std::string, Sprite> *_sprites);
    void parseBone(pugi::xml_node &node, Bone *_parent, std::map<std::string, Sprite> *_sprites);
    void createSkelet(const std::string &_path, const std::string &_name, std::map<std::string, Sprite> *_sprites);


    std::string name;
    Bone *parent = nullptr;
    std::vector<Bone*> children;
    Components component;
    // std::vector<Components> animation;
};
    // objectTransform _anchorPoint


