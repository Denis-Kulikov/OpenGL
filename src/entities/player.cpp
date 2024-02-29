#pragma once
#include <entities/player.hpp>

player::player(const std::string &_name) : name(_name) {}

bool createCharacter(const std::string &_name, const objectTransform &_trans)
{
    character = new Character(const std::string &_name, const objectTransform &_trans);
}

void attachCharacter(Character *_character) 
{
    character = _characte;
}

