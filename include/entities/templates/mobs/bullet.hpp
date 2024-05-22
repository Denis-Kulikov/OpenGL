#pragma once

#include "../../character.hpp"

class Bullet : public Pawn
{
public:
    Bullet();
    ~Bullet();

    struct Args {
        GLfloat deg;
    } args;
    
    struct Params {
        int HP = max_hp;
        int Damage = max_hp;
    } params;

    static void Initialize();
    static void SetMotion();

    virtual size_t GetSkeletSize() override;
    virtual Bone *GetSkelet() override;
    virtual std::map<std::string, Sprite> *GetSprites() override;
    virtual std::string *GetName() override;


    void Update();
    void DealingDamage(int damage);

    int id = -1;
    static inline float lifetime = 2e9;

    static inline std::string name = "NoName";
    static inline size_t skeletSize = 0;
    static inline Bone skelet;
    static inline std::map<std::string, Sprite> Sprites;
    static inline Motion motion;

    Vector3<GLfloat> color = Vector3<GLfloat>(1.0, 1.0, 0.0);
    static inline const int max_hp = 25;
};
