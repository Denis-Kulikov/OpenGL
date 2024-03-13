#pragma once

#include "pawn.hpp"
#include "../render/camera.hpp"

template<typename Derived>
class Character : public Pawn<Derived>
{
public:
    Character(const std::string &path);
};
