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

    void pushObject(Sprite* _sprite) { sprites.push_back(_sprite); };

    void deleteObject(Sprite* _sprite) {};

    std::vector<Sprite*>::iterator getIterator() { return sprites.begin(); }

    std::string name;
    std::vector<Sprite*> sprites;
};
