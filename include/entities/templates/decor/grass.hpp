#pragma once

#include "../../actor.hpp"

class Grass : public Actor
{
public:
    Grass();
    ~Grass();

    static void Initialize();
    static void SetMotion();
    
    virtual size_t GetSkeletSize() const override;
    virtual Bone *GetSkelet() const override;
    virtual std::map<std::string, Sprite> *GetSprites() const override;
    virtual std::string *GetName() const override;

    static inline std::string name = "NoName";
    static size_t skeletSize;
    static Bone skelet;
    static std::map<std::string, Sprite> Sprites;
    static inline Motion motion;
};
