#pragma once

#include "../object/sprite.hpp"
#include "../entities/components/bone.hpp"

#define NODE_STR_LENGTH 16

struct NODE_STR {
    char NAME[NODE_STR_LENGTH];
    char BONE[NODE_STR_LENGTH];
};

class Actor 
{
public:
    Actor(const std::string &path);

    void addComponent();
    std::vector<Component*> getActorComponents(Bone *_parent);

    bool loadAnimation(const std::string &_path, const std::string &_name);
    bool loadActor(const std::string &path);
    static bool loadSprites(const std::string &path);


    std::string name;
    objectTransform trans;
    Bone skelet;
    std::map<std::string, Sprite> *sprites = nullptr;
protected:
    static void parseAnimation(pugi::xml_node &_node, Bone *_bone);
    static std::map<std::string, Sprite> Sprites;
    static const struct NODE_STR NODE;
};

    // Model model; 
    // std::vector<overallAnimation> overallAnimation; 
