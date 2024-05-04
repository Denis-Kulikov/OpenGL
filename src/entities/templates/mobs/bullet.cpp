#include <entities/actor.hpp>
#include <entities/templates/mobs/bullet.hpp>


Bullet::Bullet()
    : Pawn (std::string("mobs/bullet"), GetSkeletSize())
{
    name = "Bullet";
    motionPtr = &motion;
    speed *= 2;
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
    motion = Motion();
    motion.PushSkelet(&Bullet::skelet);

    [[maybe_unused]] Motion::FunType stand = [&motion]() {};

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
}

void Bullet::DealingDamage(int damage) {
    params.HP -= damage;
}