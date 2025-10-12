#pragma once
#include "../../actor.hpp"
#include <object/mesh/skeletal_mesh.hpp>

class DualQuatSkining : public Actor
{
public:
    DualQuatSkining();
    ~DualQuatSkining();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static std::string name;
};
