#include <entities/actor.hpp>
#include <entities/templates/playable/Wilson.hpp>

std::map<std::string, Sprite> Wilson::Sprites;
Bone Wilson::skelet;
size_t Wilson::skeletSize;

Wilson::Wilson()
    : Character<Wilson>(std::string("player/Wilson"))
{
    name = "Wilson";
    std::cout << skelet.children[0]->children[0]->name << std::endl;
    
}

Wilson::~Wilson()
{
    std::cout << "Wilson::~Wilson()" << std::endl;
}

void Wilson::Initialize()
{
    std::string path("player/Wilson");
    loadSkelet(path);
    loadSprites(path);
    loadAnimation(path, "stand");
    
    auto it = skelet.children[0]->children[0]->Animations.find("stand");

    if (it != skelet.children[0]->children[0]->Animations.end()) {
        Animation& animation = it->second;
        Motion::rule myRuleAdd = {20.0, Motion::FUNTIONS::ADD};
        Motion::rule myRuleMultiply = {2.0, Motion::FUNTIONS::MULTIPLY};
        animation.motion.ruleFlip.push_back(myRuleAdd);
        animation.motion.ruleFlip.push_back(myRuleMultiply);
    }

    loadAnimation(path, "stand_2");

    std::cout << "Wilson skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}
