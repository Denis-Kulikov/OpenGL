#pragma once

#include "../../pawn.hpp"
#include <object/transform/rigid_transform.hpp>

class WoodenBox : public Pawn
{
public:
    WoodenBox(BulletManager *btManagere);
    ~WoodenBox();
    
    static void Initialize();

    virtual std::string GetName() const override;
    Mesh *GetMesh() const override;

protected:
    inline static std::string name;
    inline static Mesh *mesh = nullptr;
};
