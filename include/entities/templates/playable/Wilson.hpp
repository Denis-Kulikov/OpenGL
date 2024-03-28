#pragma once

#include "../../character.hpp"

class Wilson : public Character
{
public:
    Wilson();
    ~Wilson();
    
    static void Initialize();

    virtual size_t GetSkeletSize() override;
    virtual Bone *GetSkelet() override;
    virtual std::map<std::string, Sprite> *GetSprites() override;

    static inline std::string name = "NoName";
    static size_t skeletSize;
    static Bone skelet;
    static std::map<std::string, Sprite> Sprites;
};
