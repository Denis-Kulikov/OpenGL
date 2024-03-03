#pragma once

#include "../object/sprite.hpp"
#include "../entities/components/bone.hpp"

class Actor 
{
public:
    Actor(const std::string &_name, const objectTransform &_trans);
    Actor(const std::string &path);

    std::string name;
    objectTransform trans;

    void addComponent();
    bool loadComponents(const std::string &path);
    bool loadActor(const std::string &path);

    Bone skelets; // skelets=&overallSkelets
    // Model model; 
    // std::vector<overallAnimation> overallAnimation; 
};
