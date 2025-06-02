#pragma once

#include "../../actor.hpp"
#include <object/primitive/sphere.hpp>
#include <object/primitive/cube.hpp>
#include <object/component/template/shape.hpp>

class BrickSphere : public Actor
{
public:
    // WoodenBox(BulletManager *btManagere);
    BrickSphere();
    ~BrickSphere();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static std::string name;
    inline static Cube *cube = nullptr;
    inline static Sphere<10> *sphere = nullptr;
};
