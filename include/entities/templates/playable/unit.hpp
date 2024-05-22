#pragma once

#include "../../character.hpp"
#include <entities/templates/mobs/bullet.hpp>

class Unit : public Character
{
public:
    Unit(int id_);
    Unit();
    ~Unit();

    struct Args {
        GLfloat deg;
    } args;
    
    struct Params {
        int HP = max_hp;
        int Damage = max_hp / 3 + 1;
    } params;

    static void Initialize();
    static void SetMotion();

    virtual size_t GetSkeletSize() override;
    virtual Bone *GetSkelet() override;
    virtual std::map<std::string, Sprite> *GetSprites() override;
    virtual std::string *GetName() override;


    void Update();
    void SetID(int id_);
    Bullet* Fire();
    bool isAlive();
    void DealingDamage(int damage);


    static inline std::string name = "NoName";
    static inline size_t skeletSize = 0;
    static inline Bone skelet;
    static inline std::map<std::string, Sprite> Sprites;
    static inline Motion motion;

    Vector3<GLfloat> color = Vector3<GLfloat>(1.0, 1.0, 1.0);
    bool isFire = false;
    int id = -1;
    float last_fire = 0;

    static inline const float rate_fire = 5;
    static inline const int max_hp = 100;
};
