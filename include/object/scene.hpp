#pragma once
#include "sprite.hpp"
#include "sphere.hpp"
#include "line.hpp"
#include "cube_wire.hpp"
#include "../entities/actor.hpp"
#include "../mesh/shape.hpp"
// #include <game/bulletManager.hpp>

class Scene 
{
public:
    Scene() {};
    ~Scene() {
        for (auto& it : pawns) delete it;
    };
    void pushObject(Pawn* pawn) { pawns.push_back(pawn); };
    void pushObject(Shape* shape) { shapes.push_back(shape); };
    
    std::vector<Pawn*> pawns;
    std::vector<Shape*> shapes;
    // BulletManager btManager;
    Cube *skybox;
};
