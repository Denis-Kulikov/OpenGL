#include <entities/components/Bone.hpp>

const char Bone::BONE[] = "bone";
const char Bone::NAME[] = "name";

void Bone::addChildBone(pugi::xml_node &node, std::string _name, Bone *_parent)
{
    Bone *child = new Bone(_name, _parent);
    _parent->children.push_back(child);
    child->parseBone(node, child);
};

void Bone::parseBone(pugi::xml_node &node, Bone *_parent)
{
    for (pugi::xml_node boneNode = node.child(Bone::BONE); boneNode; boneNode = boneNode.next_sibling(Bone::BONE)) {
        std::string boneName = boneNode.attribute(Bone::NAME).as_string();
        _parent->addChildBone(boneNode, boneName, _parent);
    }
}

void Bone::createSkelet(const std::string &_path, const std::string &_name)
{
    std::string full_path = "assets/entities/" + _path + "/models/skelet/" + _name + ".xml";
    pugi::xml_document doc;
    pugi::xml_node node;
    pugi::xml_parse_result parse_result = doc.load_file(full_path.c_str());

    if (!parse_result) {
        std::cout << "Error: file not found (" << full_path << ")" << std::endl;
        return;
    }

    node = doc.child("skelet");
    name = node.attribute(Bone::NAME).as_string();

    parseBone(node, this);
}
