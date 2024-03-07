#pragma once

#include "../object/sprite.hpp"
#include "../entities/components/bone.hpp"
// #include "../entities/components/components.hpp"

#define NODE_STR_LENGTH 16

struct NODE_STR {
    char NAME[NODE_STR_LENGTH];
    char BONE[NODE_STR_LENGTH];
};

class Actor 
{
public:
    // Actor(const std::string &_name, const objectTransform &_trans);
    Actor(const std::string &path);

    static const struct NODE_STR NODE;

    std::string name;
    objectTransform trans;

    void addComponent();
    // std::vector<Sprite*> sortSpritesZ(Bone *parent)
    std::vector<Sprite*> getActorComponents(Bone *parent);
    // bone следит за связанностью
    // components привязывает спрайт и tran
    // getActorComponents возвращает вектор спрайтво из отсотрированного вектора components по Z

    bool loadAnimation(const std::string &_path, const std::string &_name);
    bool loadComponents(const std::string &path);
    bool loadActor(const std::string &path);
    static bool loadSprites(const std::string &path);

    Bone skelet; // skelet=&overallSkelets
    static std::map<std::string, Sprite> sprites;
    // Model model; 
    // std::vector<overallAnimation> overallAnimation; 
protected:
    // void SetChildAnimation(pugi::xml_node &_node);
    void parseAnimation(pugi::xml_node &_node, Bone *_bone);

};
