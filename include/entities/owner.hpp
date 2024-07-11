#pragma once

#include "character.hpp"

class Owner
{
public:
    Owner();
    void UpdateCamera();

    Camera *camera = nullptr;
    Character *player = nullptr;

    float yaw = 0;
    float pitch = 0;
};
