#pragma once
#include "../../object/objectTransform.hpp"
#include <pugixml.hpp>

class Bone
{
public:
    Bone(const std::string &_name, Bone *_parent) : name(_name), parent(_parent) {};
    Bone(const std::string &_name) : name(_name) {};
    Bone() {};

    static const char BONE[];
    static const char NAME[];
    std::string name;

    void addChildBone(std::string _name, Bone &_parent)
    {
        Bone child(_name, &_parent);
        std::cout << child.name << std::endl;
        children.push_back(child);
    };

    void parseBone(pugi::xml_node &node, Bone &parentBone) {
        for (pugi::xml_node boneNode = node.child(Bone::BONE); boneNode; boneNode = boneNode.next_sibling(Bone::BONE)) {
            std::string boneName = boneNode.attribute(Bone::NAME).as_string();
            Bone newBone(boneName);
            parentBone.addChildBone(boneName, newBone);
            parseBone(boneNode, newBone);
        }
    }

    void createSkelet(const std::string &_path, const std::string &_name) {
        std::cout << __FUNCTION__  << std::endl;
        std::string full_path = "assets/entities/" + _path + "/models/skelet/" + _name + ".xml";
        pugi::xml_document doc;
        pugi::xml_parse_result parse_result = doc.load_file(full_path.c_str());

        if (!parse_result) {
            std::cout << "Error: file not found (" << full_path << ")" << std::endl;
            return;
        }

        parseBone(doc, *this);
    }


    Bone *parent = nullptr;
    std::vector<Bone> children;
};
    // int zIndex;
    // objectTransform _anchorPoint


