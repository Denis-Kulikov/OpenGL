#include <entities/actor.hpp>
#include <entities/templates/decor/Symbol.hpp>

std::map<std::string, Sprite> Symbol::Sprites;
Bone Symbol::skelet;
size_t Symbol::skeletSize;

Symbol::Symbol()
    : Actor(std::string("decor/symbol"), GetSkeletSize())
{
    name = "Symbol";
    motionPtr = &motion;
}

Symbol::~Symbol()
{}

void Symbol::Initialize()
{
    std::string path("decor/symbol");
    name = "symbol";
    std::vector<std::string> _animations = {"stand"};
    Actor::Initialize<Symbol>(path, name, _animations);

    std::cout << "Symbol skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}

void Symbol::SetMotion()
{
    motion = Motion();
    motion.PushSkelet(&skelet);

    Motion::FunType stand = [&]() {};

    std::pair<float, Motion::FunType> _stand = {1.0, stand};
    motion.PushMotion("stand", _stand);
}

size_t Symbol::GetSkeletSize() const {
    return Symbol::skeletSize;
}

Bone *Symbol::GetSkelet() const {
    return &Symbol::skelet;
}

std::map<std::string, Sprite> *Symbol::GetSprites() const {
    return &Symbol::Sprites;
}

std::string *Symbol::GetName() const {
    return &Symbol::name;
}
