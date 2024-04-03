#pragma once

#include "../../actor.hpp"

class Wave : public Actor
{
public:
    Wave();
    ~Wave();

    static void Initialize();
    
    virtual size_t GetSkeletSize() override;
    virtual Bone *GetSkelet() override;
    virtual std::map<std::string, Sprite> *GetSprites() override;
    virtual std::string *GetName() override;

    static inline std::string name = "NoName";
    static size_t skeletSize;
    static Bone skelet;
    static std::map<std::string, Sprite> Sprites;
};
