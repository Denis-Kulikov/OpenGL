#include <entities/actor.hpp>
#include <entities/templates/mobs/spider.hpp>


Spider::Spider() : Pawn(std::string("mobs/spider"), GetSkeletSize())
{
    name = "Spider";
    motionPtr = &motion;
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
    motion = Motion();
    motion.PushSkelet(&skelet);

    [[maybe_unused]] Motion::FunType stand = [&motion]() {
        int size = skeletSize;
        Motion::bone_attribute *T = motion.transformations;
        std::fill(reinterpret_cast<float*>(&T[0]), reinterpret_cast<float*>(&T[size]), static_cast<float>(0.0));
        
        float _time = *motion.FindUniformFloat("time"); 
        static size_t spider = motion.FindBone("spider");
        

        T[spider].flip = sin(_time * M_PI * 2) * 3;
    };

    std::pair<float, Motion::FunType> _stand = {1.0, stand};
    motion.PushMotion("stand", _stand);
}

size_t Spider::GetSkeletSize() const {
    return Spider::skeletSize;
}

Bone *Spider::GetSkelet() const {
    return &Spider::skelet;
}

std::map<std::string, Sprite> *Spider::GetSprites() const {
    return &Spider::Sprites;
}

std::string *Spider::GetName() const {
    return &Spider::name;
}
