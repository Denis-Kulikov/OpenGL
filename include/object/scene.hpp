#pragma once
#include "sprite.hpp"
#include "sphere.hpp"
#include "line.hpp"
#include "cube_wire.hpp"
#include "../entities/actor.hpp"
#include "../mesh/primitive_mesh.hpp"

class Scene 
{
public:
    Scene() {};
    ~Scene() {
        for (auto& it : actors) delete it;
    };
    void pushObject(Actor* actor) { actors.push_back(actor); };
    void pushObject(Primitive_mesh* primitive) { primitives.push_back(primitive); };
    
    std::vector<Actor*> actors;
    std::vector<Primitive_mesh*> primitives;
    Cube *skybox;
};
