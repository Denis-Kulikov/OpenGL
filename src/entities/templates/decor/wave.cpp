#include <entities/actor.hpp>
#include <entities/templates/decor/wave.hpp>

std::map<std::string, Sprite> Wave::Sprites;
Bone Wave::skelet;
size_t Wave::skeletSize;

Wave::Wave()
    : Actor(std::string("decor/wave"), GetSkeletSize())
{
    name = "wave";
    motionPtr = &motion;
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
    motion = Motion();
    motion.PushSkelet(&skelet);

    Motion::FunType stand = [&]() {
        static float *__time = motion.FindUniformFloat("time"); 
        static int size = Wave::skeletSize;
        static size_t wave = motion.FindBone("wave");
        Motion::bone_attribute *T = motion.transformations;
        float _time = *__time; 

        std::fill(reinterpret_cast<float*>(&T[0]), reinterpret_cast<float*>(&T[size]), static_cast<float>(0.0));

        T[wave].offset[X] = T[wave].offset[X] = -_time;
        T[wave].offset[X] = sin(T[wave].offset[X]) * 0.66;
        T[wave].offset[Y] = cos(T[wave].offset[Y]) * 0.4;

        T[wave].flip = sin(_time) * 5.0;

        T[wave].scale[X] = T[wave].scale[Y] = 1 + _time;
        T[wave].scale[X] = sin(T[wave].scale[X]) * 0.1;
        T[wave].scale[Y] = sin(T[wave].scale[Y]) * 0.2;
    };

    std::pair<float, Motion::FunType> _stand = {2 * M_PI, stand};
    motion.PushMotion("stand", _stand);
}

size_t Wave::GetSkeletSize() const {
    return Wave::skeletSize;
}

Bone *Wave::GetSkelet() const {
    return &Wave::skelet;
}

std::map<std::string, Sprite> *Wave::GetSprites() const {
    return &Wave::Sprites;
}

std::string *Wave::GetName() const {
    return &Wave::name;
}
