#include <entities/actor.hpp>
#include <entities/templates/playable/unit.hpp>


Unit::Unit()
    : Character(std::string("player/unit"), GetSkeletSize())
{
    name = "Unit";
    motionPtr = &motion;
}

Unit::~Unit()
{}

void Unit::Initialize()
{
    std::string path("player/unit");
    name = "Unit";
    std::vector<std::string> _animations = {"stand"};
    Actor::Initialize<Unit>(path, name, _animations);

    std::cout << "Unit skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}

void Unit::SetMotion()
{
    auto* vec = new std::vector<std::pair<std::string, float>>;
    vec->push_back(std::pair<std::string, float>("flip", 20.0));
    motion = Motion(vec);
    motion.PushSkelet(&Unit::skelet);

    [[maybe_unused]] Motion::FunType stand = [&motion]() {
        int size = skeletSize;
        Motion::bone_attribute *T = motion.transformations;
        std::fill(reinterpret_cast<float*>(&T[0]), reinterpret_cast<float*>(&T[size]), static_cast<float>(0.0));
        
        float _time = *motion.FindUniformFloat("time"); 
        float flip = *motion.FindUniformFloat("flip"); 
        static size_t body = motion.FindBone("body");

        T[body].flip = flip;
    };

    std::pair<float, Motion::FunType> _stand = {2.0, stand};
    motion.PushMotion("stand", _stand);
}

size_t Unit::GetSkeletSize() {
    return Unit::skeletSize;
}

Bone *Unit::GetSkelet() {
    return &Unit::skelet;
}

std::map<std::string, Sprite> *Unit::GetSprites() {
    return &Unit::Sprites;
}

std::string *Unit::GetName() {
    return &Unit::name;
}


void Unit::Update(GLfloat deg) {
    static float& flip = *motion.FindUniformFloat("flip"); 
    flip = fmod(deg, 360);

    Actor::SetSpriteColor(this, std::string("circle_bone"), color);
}