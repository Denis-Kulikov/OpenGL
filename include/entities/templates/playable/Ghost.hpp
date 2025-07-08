#pragma once

#include "../../actor.hpp"

class Ghost : public Actor
{
public:
    Ghost();
    ~Ghost();
    
    static void Initialize();

    std::string GetName() const override;
    
protected:
    inline static std::string name;
};
