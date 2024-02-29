#pragma once
#include "actor.hpp"
#include "../render/camera.hpp"

class Character : public Actor
{
public:
    Character(const std::string &_name, const objectTransform &_trans) : Actor(const std::string &_name, const objectTransform &_trans);
    bool createCamera();
    void attachCamera(Camera _camera);
    
    Camera *camera;
};
