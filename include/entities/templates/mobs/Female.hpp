#pragma once

#include "../../character.hpp"
#include <object/transform/rigid_transform.hpp>

class Female : public Character
{
public:
    Female(BulletManager *btManagere);
    ~Female();
    
    static void Initialize();

    virtual std::string GetName() const override;
    Mesh *GetMesh() const override;

protected:
    inline static std::string name;
    inline static Mesh *mesh = nullptr;
};
