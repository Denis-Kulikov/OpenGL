#pragma once

#include "../../pawn.hpp"

class Floor : public Pawn
{
public:
    Floor();
    ~Floor();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static std::string name;
};
