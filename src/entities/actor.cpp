#pragma once
#include <entities/actor.hpp>

Actor::Actor(const std::string &_name, const objectTransform &_trans)
    : name(_name), trans(_trans)
{

}
