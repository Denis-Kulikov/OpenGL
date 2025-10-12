#pragma once
#include "../../actor.hpp"
#include <object/mesh/skeletal_mesh.hpp>

class Female : public Actor
{
public:
    Female();
    ~Female();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static std::string name;
};
