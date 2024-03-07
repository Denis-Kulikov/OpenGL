#pragma once

#include <pugixml.hpp>
#include "../../object/objectTransform.hpp"
#include "components.hpp"

class Bone
{
public:
    Bone(const std::string &_name, Bone *_parent) : name(_name), parent(_parent) {};
    Bone(const std::string &_name) : name(_name) {};
    Bone() {};

    static const char BONE[];
    static const char NAME[];
    std::string name;

    void addChildBone(std::string _name, Bone *_parent, std::map<std::string, Sprite> *_sprites)
    {
        Bone *child = new Bone(_name, _parent);
        auto it = _sprites->find(child->name); // Поиск спрайта по имени в map
        if (it != _sprites->end()) {
            child->component.sprite = &(it->second); // Присвоение указателя на спрайт, если найден
            std::cout << child->name << "\t" << child->parent->name << std::endl;
            children.push_back(child);
        }
        // children.push_back(child);
    };

    void parseBone(pugi::xml_node &node, Bone *_parent, std::map<std::string, Sprite> *_sprites) {
        std::cout << __FUNCTION__  << std::endl;
        for (pugi::xml_node boneNode = node.child(Bone::BONE); boneNode; boneNode = boneNode.next_sibling(Bone::BONE)) {
            std::string boneName = boneNode.attribute(Bone::NAME).as_string();
            // Bone newBone(boneName);
            _parent->addChildBone(boneName, this, _sprites);
            parseBone(boneNode, this, _sprites);
        }
    }

    void createSkelet(const std::string &_path, const std::string &_name, std::map<std::string, Sprite> *_sprites) {
        // std::cout << __FUNCTION__  << std::endl;

        if (_sprites == nullptr) {
            std::cout << "Not found &sprites" << std::endl;
            return;
        }

        std::string full_path = "assets/entities/" + _path + "/models/skelet/" + _name + ".xml";
        pugi::xml_document doc;
        pugi::xml_parse_result parse_result = doc.load_file(full_path.c_str());

        if (!parse_result) {
            std::cout << "Error: file not found (" << full_path << ")" << std::endl;
            return;
        }

        parseBone(doc, this, _sprites);
    }

    // void pushSprites()
    // {
        // sprites = _sprites;   
    // }


    Bone *parent = nullptr;
    std::vector<Bone*> children;
    Components component;
    // std::map<std::string, Sprite> *sprites;
    // std::vector<Components> animation;
};
    // int zIndex;
    // objectTransform _anchorPoint


