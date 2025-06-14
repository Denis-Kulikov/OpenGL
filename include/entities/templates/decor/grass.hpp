#pragma once

#include "../../actor.hpp"
#include <object/geometry/primitive/sprite.hpp>
#include <object/component/template/shape.hpp>

class Grass : public Actor
{
public:
    Grass();
    ~Grass();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static std::string name;
};
