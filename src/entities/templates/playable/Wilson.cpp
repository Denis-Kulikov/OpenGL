#include <entities/actor.hpp>
#include <entities/templates/playable/Wilson.hpp>


Wilson::Wilson()
    : Character(std::string("player/Wilson"), GetSkeletSize())
{
    name = "Wilson";
    motionPtr = &_motion;
    _motion.PushTransform(animationInfo.transformations);
}

Wilson::~Wilson()
{}

void Wilson::Initialize()
{
    std::string path("player/Wilson");
    name = "Wilson";
    std::vector<std::string> _animations = {"stand", "stand_2"};
    Actor::Initialize<Wilson>(path, name, _animations);

    std::cout << "Wilson skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}

void Wilson::SetMotion()
{
    _motion = motion();

    [[maybe_unused]] motion::FunType stand = [&_motion]() {
        int size = Wilson::skeletSize;
        float *transform = _motion.transformations;
        std::fill(&transform[0], &transform[5 * size], 0.0);
        
        // float _time = *_motion.FindUniformFloat("time"); 

        // transform[5] = transform[6] = _time * (-1);
        // transform[5] = sin(transform[5]) * 0.66;
        // transform[6] = cos(transform[6]) * 0.4;

        // transform[7] = sin(_time) * 5.0;

        // transform[8] = transform[9] = 1 + _time;
        // transform[8] *= 0.1;
        // transform[9] *= 0.2;
    };

    std::pair<float, motion::FunType> _stand = {0.0, stand};
    _motion.PushMotion("stand", _stand);
}

size_t Wilson::GetSkeletSize() {
    return Wilson::skeletSize;
}

Bone *Wilson::GetSkelet() {
    return &Wilson::skelet;
}

std::map<std::string, Sprite> *Wilson::GetSprites() {
    return &Wilson::Sprites;
}

std::string *Wilson::GetName() {
    return &Wilson::name;
}
