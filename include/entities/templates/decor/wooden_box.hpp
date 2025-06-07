#pragma once

#include "../../actor.hpp"
#include <object/geometry/primitive/cube.hpp>
#include <object/component/template/shape.hpp>

class WoodenBox : public Actor
{
public:
    // WoodenBox(BulletManager *btManagere);
    WoodenBox();
    ~WoodenBox();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static std::string name;
    inline static Cube *cube = nullptr;
};
