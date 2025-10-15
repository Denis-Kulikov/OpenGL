#pragma once

#include "../../actor.hpp"
#include <object/mesh/primitive/cube.hpp>
#include <object/component/template/mesh.hpp>

class Skybox : public Actor
{
public:
    Skybox();
    ~Skybox();
    
    ACTOR_BASE;
};
