#pragma once

#include "../../character.hpp"

class Spider : public Character
{
public:
    Spider();
    ~Spider();

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
