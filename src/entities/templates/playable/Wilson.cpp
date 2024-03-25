#include <entities/actor.hpp>
#include <entities/templates/playable/Wilson.hpp>

std::map<std::string, Sprite> Wilson::Sprites;
Bone Wilson::skelet;
size_t Wilson::skeletSize;

Wilson::Wilson()
    : Character<Wilson>(std::string("player/Wilson"))
{
    name = "Wilson";
}

Wilson::~Wilson()
{
    std::cout << "Wilson::~Wilson()" << std::endl;
}

void Wilson::Initialize()
{
    std::string path("player/Wilson");
    name = "Wilson";
    loadSkelet(path);
    loadSprites(path);
    loadAnimation(path, "stand");
    
    // auto it = skelet.children[0]->children[0]->Animations.find("stand");

    // if (it != skelet.children[0]->children[0]->Animations.end()) {
    //     Animation& animation = it->second;

    //     Motion::rule myRuleAdd1 = {20.0, Motion::FUNTIONS::ADD};
    //     Motion::rule myRuleAdd2 = {20.0, Motion::FUNTIONS::ADD};
    //     Motion::rule myRuleTime = {1.0, Motion::FUNTIONS::TIME};
    //     Motion::rule myRuleSin =  {10.0, Motion::FUNTIONS::SIN};
    //     std::vector<Motion::rule> myRuleVector1 = { myRuleAdd1 };
    //     std::vector<Motion::rule> myRuleVector2 = { myRuleAdd2, myRuleTime,  myRuleSin};

    //     std::pair<float, std::vector<Motion::rule>> frame1 = {0.5, myRuleVector2};
    //     std::pair<float, std::vector<Motion::rule>> frame2 = {1.0, myRuleVector1};
        
    //     animation.motion.ruleFlip.push_back(frame1);
    //     animation.motion.ruleFlip.push_back(frame2);
    // }

    loadAnimation(path, "stand_2");

    std::cout << "Wilson skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}
