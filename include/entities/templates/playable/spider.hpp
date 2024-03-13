#pragma once

#include "../../character.hpp"

class Spider : public Character
{
public:
    Spider() : Character(std::string("mobs/spider"))
    {
        std::string path("mobs/spider");

        std::cout << __FUNCTION__ << std::endl; 
        // std::cout << "Address: " << &Sprites << std::endl;

        Spider::loadSprites(&Sprites, path);
        skelet.createSkelet(path, "skelet");
        Spider::loadAnimation(path, "stand", &skelet, sprites);
    }
    
protected:
    static std::map<std::string, Sprite> Sprites;

};
