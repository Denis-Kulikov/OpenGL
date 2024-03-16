#pragma once

#include "../../character.hpp"

class Spider : public Character<Spider>
{
public:
    Spider() : Character(std::string("mobs/spider"))
    {
        components = new Component[skeletSize];
        animations = new Animation*[skeletSize];
        for (int i = 0; i < skeletSize; i++) {
            components[i] = Component();
            animations[i] = nullptr;
        }
        animations[0] = new Animation();
    }


    ~Spider()
    {
    }


    static void Initialize()
    {
        std::string path("mobs/spider");
        loadSkelet(path);
        loadSprites(path);
        loadAnimation(path, "stand");

        std::cout << "Spider skelet size: " << skeletSize << std::endl;
        skelet.printBones(0);
        std::cout << std::endl;
    }
    
    static size_t skeletSize;
    static Bone skelet;
    static std::map<std::string, Sprite> Sprites;
};
