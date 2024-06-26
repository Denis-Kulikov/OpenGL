#pragma once

#include "../../character.hpp"

class Wilson : public Character
{
public:
    Wilson();
    ~Wilson();
    
    static void Initialize();
    static void SetMotion();

    virtual size_t GetSkeletSize() const override;
    virtual Bone *GetSkelet() const override;
    virtual std::map<std::string, Sprite> *GetSprites() const override;
    virtual std::string *GetName() const override;

    static inline std::string name = "NoName";
    static inline size_t skeletSize = 0;
    static inline Bone skelet;
    static inline std::map<std::string, Sprite> Sprites;
    static inline Motion motion;
};
