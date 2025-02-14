#pragma once

#include "../../pawn.hpp"
#include <object/transform/rigid_transform.hpp>

class Floor : public Pawn
{
public:
    Floor(BulletManager *btManagere);
    ~Floor();
    
    static void Initialize();

    std::string GetName() const override;
    Mesh *GetMesh() const override;

protected:
    inline static std::string name;
    inline static Mesh *mesh = nullptr;
};
