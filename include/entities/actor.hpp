#pragma once

#include "../object/sprite.hpp"
#include "../entities/components/bone.hpp"

template <typename Derived>
class Actor 
{
public:
    Actor(const std::string &path);

    void addComponent();
    std::vector<Component*> getActorComponents(Bone *_parent);

    static bool loadAnimation(const std::string &_path, const std::string &_name, Bone *_skelet, std::map<std::string, Sprite> *_Sprites);
    bool loadActor(const std::string &path);
    static bool loadSprites(std::map<std::string, Sprite> *_Sprites, const std::string &path);


    std::string name;
    objectTransform trans;
    Bone skelet;
    std::map<std::string, Sprite> *sprites = nullptr;
    static std::map<std::string, Sprite> Sprites;
protected:
    static void parseAnimation(pugi::xml_node &_node, Bone *_bone, std::map<std::string, Sprite> *_Sprites);
};
