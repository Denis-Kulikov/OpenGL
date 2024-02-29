#pragma once
#include "character.hpp"

class player 
{
public:
    player(const std::string &_name);
    std::string name;
    Character *character;

    bool createCharacter(const std::string &_name, const objectTransform &_trans) : Character(const std::string &_name, const objectTransform &_trans);
    void attachCharacter(Character *_character);
};
