#pragma once

#include "../../character.hpp"

class Female : public Character
{
public:
    Female();
    ~Female();
    
    static void Initialize();

    virtual std::string GetName() const override;
    Mesh *GetMesh() const override;

protected:
    inline static std::string name;
    inline static Mesh *mesh = nullptr;
};
