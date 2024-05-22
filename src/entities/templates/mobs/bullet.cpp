#include <entities/actor.hpp>
#include <entities/templates/mobs/bullet.hpp>


Bullet::Bullet()
    : Pawn (GetSkeletSize())
{
    name = "Bullet";
    motionPtr = &motion;
    speed *= 1.5;
}

Bullet::~Bullet()
{}

void Bullet::Initialize()
{
    std::string path("mobs/bullet");
    name = "Bullet";
    std::vector<std::string> _animations = {"stand"};
    Actor::Initialize<Bullet>(path, name, _animations);

    std::cout << "Bullet skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}

void Bullet::SetMotion()
{
    auto* vec_int = new std::vector<std::pair<std::string, int>>;
    vec_int->push_back(std::pair<std::string, int>("HP", max_hp));

    motion = Motion(vec_int);
    motion.PushSkelet(&Bullet::skelet);

    [[maybe_unused]] Motion::FunType stand = [&motion]() {
        Motion::bone_attribute *T = motion.transformations;
        int hp = *motion.FindUniformInt("HP"); 
        static size_t HP = motion.FindBone("HP");
        static size_t scale = motion.FindBone("scale");

        if (hp >= max_hp) {
            T[scale].scale[0] = -1.0f;
        } else {
            T[HP].scale[0] = static_cast<GLfloat>(hp) / static_cast<GLfloat>(max_hp) - 1;
            T[HP].offset[0] = 1 - static_cast<GLfloat>(hp) / static_cast<GLfloat>(max_hp);
        }
    };

    std::pair<float, Motion::FunType> _stand = {2.0, stand};
    motion.PushMotion("stand", _stand);
}

size_t Bullet::GetSkeletSize() {
    return Bullet::skeletSize;
}

Bone *Bullet::GetSkelet() {
    return &Bullet::skelet;
}

std::map<std::string, Sprite> *Bullet::GetSprites() {
    return &Bullet::Sprites;
}

std::string *Bullet::GetName() {
    return &Bullet::name;
}


void Bullet::Update() {
    Actor::SetSpriteColor(this, std::string("circle_bone"), color);
    static int& hp = *motion.FindUniformInt("HP"); 
    Vector3<GLfloat> hp_color;
    hp = params.HP;
    if (params.HP >= 50) {
        hp_color = Vector3<GLfloat>(static_cast<GLfloat>(max_hp - params.HP) / max_hp * 2, 1.0, 0.0);
    } else {
        hp_color = Vector3<GLfloat>(1.0, static_cast<GLfloat>(params.HP) / max_hp * 2, 0.0);
    }
    Actor::SetSpriteColor(this, std::string("hp"), hp_color);
}

void Bullet::DealingDamage(int damage) {
    params.HP -= damage;
}