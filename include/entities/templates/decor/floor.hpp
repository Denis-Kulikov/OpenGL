#pragma once

#include "../../pawn.hpp"

class Floor : public Pawn
{
public:
    Floor();
    ~Floor();
    
    static void Initialize();

    std::string GetName() const override;
    Mesh *GetMesh() const override;

protected:
    inline static std::string name;
    inline static Mesh *mesh = nullptr;
};
