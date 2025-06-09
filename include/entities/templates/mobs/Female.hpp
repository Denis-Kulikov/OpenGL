#pragma once
#include "../../actor.hpp"
#include <object/geometry/skeletal_mesh.hpp>

class Female : public Actor
{
public:
    Female();
    ~Female();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static GeometrySkeletalMesh* mesh = nullptr;
    inline static std::string name;
};
