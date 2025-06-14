#pragma once

#include "../../actor.hpp"
#include <object/geometry/primitive/sphere.hpp>

class BrickSphere : public Actor
{
public:
    BrickSphere();
    ~BrickSphere();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static std::string name;
};
