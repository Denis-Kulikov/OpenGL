#pragma once

#include "../object/sprite.hpp"
#include "../entities/components/bone.hpp"
// #include "../entities/components/components.hpp"

class Actor 
{
public:
    // Actor(const std::string &_name, const objectTransform &_trans);
    Actor(const std::string &path);

    std::string name;
    objectTransform trans;

    void addComponent();
    // std::vector<Sprite*> sortSpritesZ(Bone *parent)
    std::vector<Sprite*> getActorComponents(Bone *parent);
    // bone следит за связанностью
    // components привязывает спрайт и tran
    // getActorComponents возвращает вектор спрайтво из отсотрированного вектора components по Z

    bool loadAnimation(const std::string &path);
    bool loadComponents(const std::string &path);
    bool loadActor(const std::string &path);
    static bool loadSprites(const std::string &path);

    Bone skelets; // skelets=&overallSkelets
    static std::map<std::string, Sprite> sprites;
    // Model model; 
    // std::vector<overallAnimation> overallAnimation; 

};
