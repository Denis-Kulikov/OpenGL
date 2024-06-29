#include <entities/templates/playable/Wilson.hpp>

Wilson::Wilson()
    : Character(std::string("player/Wilson"), GetSkeletSize())
{
    name = "Wilson";
    motionPtr = &motion;
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
    motion = Motion();
    motion.PushSkelet(&Wilson::skelet);

    [[maybe_unused]] Motion::FunType stand = [&]() mutable {
        int size = skeletSize;
        Motion::bone_attribute *T = motion.transformations;
        std::fill(reinterpret_cast<float*>(&T[0]), reinterpret_cast<float*>(&T[size]), static_cast<float>(0.0));
        
        float _time = *motion.FindUniformFloat("time"); 
        static size_t head = motion.FindBone("head");
        static size_t shoulder_left = motion.FindBone("shoulder_left");
        static size_t arm_left = motion.FindBone("arm_left");
        static size_t hand_left = motion.FindBone("hand_left");
        static size_t shoulder_right = motion.FindBone("shoulder_right");
        static size_t arm_right = motion.FindBone("arm_right");
        

        T[head].flip = sin(_time * M_PI / 2) * 20;

        T[shoulder_left].flip = 30 + sin(_time * M_PI / 2) * 30;

        T[arm_left].flip = sin(_time) * 10;

        T[hand_left].flip = sin(_time) * 20;
        T[hand_left].scale[0] = T[hand_left].scale[1] = sin(_time * M_PI / 2) * 0.3;

        T[shoulder_right].flip = -10 + sin(_time * M_PI / 2) * 3;

        T[arm_right].flip = sin(_time * M_PI / 2) * 5;
    };

    std::pair<float, Motion::FunType> _stand = {2.0, stand};
    motion.PushMotion("stand", _stand);
}

size_t Wilson::GetSkeletSize() const {
    return Wilson::skeletSize;
}

Bone *Wilson::GetSkelet() const {
    return &Wilson::skelet;
}

std::map<std::string, Sprite> *Wilson::GetSprites() const {
    return &Wilson::Sprites;
}

std::string *Wilson::GetName() const {
    return &Wilson::name;
}
