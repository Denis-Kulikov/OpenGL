#pragma once

#include "../../character.hpp"

class Ghost : public Character
{
public:
    Ghost();
    ~Ghost();
    
    static void Initialize();

    std::string GetName() const override;
    
protected:
    inline static std::string name;
};
