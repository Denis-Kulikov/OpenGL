#pragma once

#include "../../character.hpp"

class Ghost : public Character
{
public:
    Ghost();
    ~Ghost();
    
    static void Initialize();
    static void SetMotion();

    virtual std::string GetName() const override;
    Mesh *GetMesh() const override;
    
protected:
    inline static std::string name;
};
