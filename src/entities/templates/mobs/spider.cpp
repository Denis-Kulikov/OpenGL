#include <entities/actor.hpp>
#include <entities/templates/mobs/spider.hpp>


Spider::Spider() : Character(std::string("mobs/spider"), GetSkeletSize())
{
    name = "Spider";
    motionPtr = &_motion;
    _motion.PushTransform(animationInfo.transformations);
}

Spider::~Spider()
{}

void Spider::Initialize()
{
    std::string path("mobs/spider");
    name = "spider";
    std::vector<std::string> _animations = {"stand"};
    Actor::Initialize<Spider>(path, name, _animations);

    std::cout << "Spider skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}

void Spider::SetMotion()
{
    _motion = motion();

    [[maybe_unused]] motion::FunType stand = [&_motion]() {};

    std::pair<float, motion::FunType> _stand = {0.0, stand};
    _motion.PushMotion("stand", _stand);
}

size_t Spider::GetSkeletSize() {
    return Spider::skeletSize;
}

Bone *Spider::GetSkelet() {
    return &Spider::skelet;
}

std::map<std::string, Sprite> *Spider::GetSprites() {
    return &Spider::Sprites;
}

std::string *Spider::GetName() {
    return &Spider::name;
}
