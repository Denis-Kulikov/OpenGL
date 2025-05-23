#pragma once
#include "../../actor.hpp"
#include <object/geometry/mesh.hpp>

class Female : public Actor
{
public:
    Female();
    ~Female();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static GeometryMesh* mesh = nullptr;
    inline static std::string name;
};
