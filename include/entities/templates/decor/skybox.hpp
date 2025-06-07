#pragma once

#include "../../actor.hpp"
#include <object/geometry/primitive/cube.hpp>
#include <object/component/template/shape.hpp>

class Skybox : public Actor
{
public:
    Skybox();
    ~Skybox();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static std::string name;
    inline static Cube *cube = nullptr;
};
