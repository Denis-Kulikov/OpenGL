#include <entities/components/Bone.hpp>

const char Bone::BONE[] = "bone";
const char Bone::NAME[] = "name";

void Bone::printBones(int lvl)
{
    for (int i = 0; i < lvl; i++) std::cout << '\t'; 
    int color = 32 + (lvl % 5);
    std::cout << "\033[" << std::to_string(color) << "m" << name << "\033[39m" << std::endl;
    for (auto it : this->children) it->printBones(lvl + 1);
}

size_t Bone::addChildBone(pugi::xml_node &node, std::string _name, Bone *_parent)
{
    Bone *child = new Bone(_name, _parent);
    _parent->children.push_back(child);
    return child->parseBone(node, child);
};

size_t Bone::parseBone(pugi::xml_node &node, Bone *_parent)
{
    size_t size = 0;
    for (pugi::xml_node boneNode = node.child(Bone::BONE); boneNode; boneNode = boneNode.next_sibling(Bone::BONE)) {
        std::string boneName = boneNode.attribute(Bone::NAME).as_string();
        size += _parent->addChildBone(boneNode, boneName, _parent);
        size++;
    }
    return size;
}

size_t Bone::createSkelet(const std::string &_path, const std::string &_name)
{
    std::string full_path = "assets/entities/" + _path + "/models/skelet/" + _name + ".xml";
    pugi::xml_document doc;
    pugi::xml_node node;
    pugi::xml_parse_result parse_result = doc.load_file(full_path.c_str());

    if (!parse_result) {
        std::cout << "Error: file not found (" << full_path << ")" << std::endl;
        return 0;
    }

    node = doc.child("skelet");
    name = node.attribute(Bone::NAME).as_string();

    return parseBone(node, this) + 1;
}
