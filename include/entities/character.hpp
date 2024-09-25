#pragma once

#include "pawn.hpp"
#include "../render/camera.hpp"

class Character : public Pawn
{
public:
    Character(const std::string &path);
};
