#pragma once
#include "sprite.hpp"

class Scene {
public:
    Scene(std::string _name) : name(_name) {};
    ~Scene() {};

    void pushObject(sprite* object) {
        objects.push_back(object);
    }

    void deleteObject(sprite* object) {
    }

    std::vector<sprite*>::iterator getIterator() {
        return objects.begin();
    }

    std::string name;
    std::vector<sprite*> objects;
};
