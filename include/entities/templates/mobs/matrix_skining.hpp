#pragma once
#include "../../actor.hpp"
#include <object/mesh/skeletal_mesh.hpp>

class MatrixSkining : public Actor
{
public:
    MatrixSkining();
    ~MatrixSkining();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static std::string name;
};
