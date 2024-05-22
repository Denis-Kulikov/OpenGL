#pragma once

#include "pawn.hpp"
#include "../render/camera.hpp"

class Character : public Pawn
{
public:
    Character(const size_t SkeletSize) : Pawn(SkeletSize) {};

    
};
