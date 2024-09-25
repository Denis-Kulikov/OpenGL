#pragma once
#include "sprite.hpp"
#include "sphere.hpp"
#include "circle.hpp"
#include "line.hpp"
#include "cube_bone.hpp"
#include "../entities/actor.hpp"

class Scene 
{
public:
    Scene() {};
    ~Scene() {
        for (auto& it : actors) delete it;
    };
    void pushObject(Actor* actor) { actors.push_back(actor); };
    void pushObject(Sprite* sprite) { sprites.push_back(sprite); };
    
    std::vector<Actor*> actors;
    std::vector<Sprite*> sprites;
    Cube *skybox;
};
