#pragma once

#include "../../actor.hpp"
#include <object/geometry/primitive/sprite.hpp>
#include <object/component/template/shape.hpp>

class StoneFloor : public Actor
{
public:
    StoneFloor();
    ~StoneFloor();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static std::string name;
};
