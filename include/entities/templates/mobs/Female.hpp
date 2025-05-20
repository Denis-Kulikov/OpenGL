#pragma once

#include "../../character.hpp"

class Female : public Character
{
public:
    Female();
    ~Female();
    
    static void Initialize();

    std::string GetName() const override;

protected:
    inline static std::string name;
};
