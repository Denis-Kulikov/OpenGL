#pragma once

#include "../../actor.hpp"
#include <object/mesh/primitive/sprite.hpp>
#include <object/component/template/mesh.hpp>

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
