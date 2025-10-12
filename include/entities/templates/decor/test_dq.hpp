#pragma once

#include "../../actor.hpp"
#include <object/mesh/primitive/cube.hpp>
#include <object/component/template/mesh.hpp>

class TestDQ : public Actor
{
public:
    TestDQ();
    ~TestDQ();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static std::string name;
};
