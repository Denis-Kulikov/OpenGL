#include <entities/actor.hpp>
#include <entities/templates/decor/wave.hpp>

std::map<std::string, Sprite> Wave::Sprites;
Bone Wave::skelet;
size_t Wave::skeletSize;

Wave::Wave()
    : Actor(std::string("decor/wave"), GetSkeletSize())
{
    name = "wave";
    motionPtr = &_motion;
    _motion.PushTransform(animationInfo.transformations);

    std::cout << "Motion: " << std::endl;
    for (const auto &it : _motion.function) {
        std::cout << it.first << std::endl;
    }
}

Wave::~Wave()
{}

void Wave::Initialize()
{
    std::string path("decor/wave");
    name = "wave";
    std::vector<std::string> _animations = {"stand"};
    Actor::Initialize<Wave>(path, name, _animations);

    std::cout << "wave skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}

void Wave::SetMotion()
{
    [[maybe_unused]] _motion = motion();

    [[maybe_unused]] motion::FunType stand = [&_motion]() {
        int size = Wave::skeletSize;
        float *transform = _motion.transformations;
        std::fill(&transform[0], &transform[5 * size], 0.0);
        
        float _time = *_motion.FindUniformFloat("time"); 

        // transform[5] = transform[6] = -_time * M_PI / 2;
        transform[5] = transform[6] = -_time;
        transform[5] = sin(transform[5]) * 0.66;
        transform[6] = cos(transform[6]) * 0.4;

        transform[7] = sin(_time) * 5.0;

        transform[8] = transform[9] = 1 + _time;
        transform[8] = sin(transform[8]) * 0.1;
        transform[9] = sin(transform[9]) * 0.2;
    };

    std::pair<float, motion::FunType> _stand = {2 * M_PI, stand};
    _motion.PushMotion("stand", _stand);
}

size_t Wave::GetSkeletSize() {
    return Wave::skeletSize;
}

Bone *Wave::GetSkelet() {
    return &Wave::skelet;
}

std::map<std::string, Sprite> *Wave::GetSprites() {
    return &Wave::Sprites;
}

std::string *Wave::GetName() {
    return &Wave::name;
}
