#pragma once

// #include "../../object/objectTransform.hpp"
// #include "../object/sprite.hpp"
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
    // void addChildBone(pugi::xml_node &node, std::string _name, Bone *_parent, std::map<std::string, Sprite> *_sprites)
    // {
    //     Bone *child = new Bone(_name, _parent);
    //     auto it = _sprites->find(child->name); 
    //     if (it != _sprites->end()) {
    //         child->component.sprite = &(it->second); 
    //         // std::cout << child->name << "\t" << child->parent->name << std::endl;
    //         _parent->children.push_back(child);
    //         child->parseBone(node, child, _sprites);
    //     }
    // };

    void parseBone(pugi::xml_node &node, Bone *_parent, std::map<std::string, Sprite> *_sprites);
    // void parseBone(pugi::xml_node &node, Bone *_parent, std::map<std::string, Sprite> *_sprites) {
    //     for (pugi::xml_node boneNode = node.child(Bone::BONE); boneNode; boneNode = boneNode.next_sibling(Bone::BONE)) {
    //         std::string boneName = boneNode.attribute(Bone::NAME).as_string();
    //         _parent->addChildBone(boneNode, boneName, _parent, _sprites);
    //     }
    // }

    void createSkelet(const std::string &_path, const std::string &_name, std::map<std::string, Sprite> *_sprites);
    // void createSkelet(const std::string &_path, const std::string &_name, std::map<std::string, Sprite> *_sprites) {

    //     if (_sprites == nullptr) {
    //         std::cout << "Not found &sprites" << std::endl;
    //         return;
    //     }

    //     std::string full_path = "assets/entities/" + _path + "/models/skelet/" + _name + ".xml";
    //     pugi::xml_document doc;
    //     pugi::xml_node node;
    //     pugi::xml_parse_result parse_result = doc.load_file(full_path.c_str());

    //     if (!parse_result) {
    //         std::cout << "Error: file not found (" << full_path << ")" << std::endl;
    //         return;
    //     }

    //     node = doc.child("skelet");
    //     name = node.attribute(Bone::NAME).as_string();

    //     parseBone(node, this, _sprites);
    // }


    std::string name;
    Bone *parent = nullptr;
    std::vector<Bone*> children;
    Components component;
    // std::vector<Components> animation;
};
    // objectTransform _anchorPoint


