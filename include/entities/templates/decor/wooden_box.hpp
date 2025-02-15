#pragma once

#include "../../pawn.hpp"
#include <object/cube.hpp>
#include <object/component/template/shape.hpp>

class WoodenBox : public Pawn
{
public:
    // WoodenBox(BulletManager *btManagere);
    WoodenBox();
    ~WoodenBox();
    
    static void Initialize();

    std::string GetName() const override;
    Mesh *GetMesh() const override;

protected:
    static void BuildSpiral(ComponentShape *parent, int step, int maxSteps);

    inline static std::string name;
    inline static Mesh *mesh = nullptr;
    inline static Cube *cube = nullptr;
};
