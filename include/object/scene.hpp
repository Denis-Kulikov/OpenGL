#pragma once
#include "sprite.hpp"
#include "sphere.hpp"
#include "circle.hpp"
#include "line.hpp"
#include "cube_bone.hpp"
#include "../entities/components/component.hpp"
#include "../entities/templates/playable/unit.hpp"
#include "../game/gameTime.hpp"

class Scene 
{
public:
    Scene(const std::string& _name, const int id) : name(_name) {
        for (int i = 0; i < MAX_PLAYERS; i++) players.insert(std::make_pair(i, Unit(i)));
        player = &players[id];
    };
    ~Scene() {};

    void pushObject(Component _component) { component.push_back(_component); };

    void deleteObject(Component _component) {};

    std::vector<Component>::iterator getIterator() { return component.begin(); }

    std::string name;
    std::vector<Component> component;
    GameTime Time;
    Unit *player = nullptr;
    std::map<int, Unit> players;
    std::list<Bullet*> bullets;
};
