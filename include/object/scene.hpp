#pragma once
#include "sprite.hpp"
#include "sphere.hpp"
#include "circle.hpp"
#include "line.hpp"
#include "cube_bone.hpp"

class Scene 
{
public:
    Scene(std::string _name) : name(_name) {};
    ~Scene() {};

    void pushObject(sprite* _sprite) { sprites.push_back(_sprite); }

    void deleteObject(sprite* _sprite) {}

    std::vector<sprite*>::iterator getIterator() { return sprites.begin(); }

    std::string name;
    std::vector<sprite*> sprites;
};
