#include <entities/actor.hpp>
#include <entities/templates/playable/unit.hpp>


Unit::Unit()
    : Character(std::string("player/unit"), GetSkeletSize())
{
    name = "Unit";
    motionPtr = &motion;
    updateAnimation("stand");
}

Unit::~Unit()
{}

void Unit::Initialize()
{
    std::string path("player/unit");
    name = "Unit";
    // std::vector<std::string> _animations = {"stand"};
    std::vector<std::string> _animations = {"stand", "kill"};
    Actor::Initialize<Unit>(path, name, _animations);

    std::cout << "Unit skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}

void Unit::SetMotion()
{
    auto* vec_float = new std::vector<std::pair<std::string, float>>;
    auto* vec_int = new std::vector<std::pair<std::string, int>>;
    vec_float->push_back(std::pair<std::string, float>("flip", 0.0));
    vec_int->push_back(std::pair<std::string, int>("HP", max_hp));
    motion = Motion(vec_float, vec_int);
    motion.PushSkelet(&Unit::skelet);

    [[maybe_unused]] Motion::FunType kill = [&motion]() {};
    std::pair<float, Motion::FunType> _kill = {1.0, kill};
    motion.PushMotion("kill", _kill);

    [[maybe_unused]] Motion::FunType stand = [&motion]() {
        int size = skeletSize;
        Motion::bone_attribute *T = motion.transformations;
        std::fill(reinterpret_cast<float*>(&T[0]), reinterpret_cast<float*>(&T[size]), static_cast<float>(0.0));
        
        float _time = *motion.FindUniformFloat("time"); 
        float flip = *motion.FindUniformFloat("flip"); 
        int hp = *motion.FindUniformInt("HP"); 
        static size_t body = motion.FindBone("body");
        static size_t HP = motion.FindBone("HP");

        T[body].flip = flip;
        T[HP].scale[0] = static_cast<GLfloat>(hp) / static_cast<GLfloat>(max_hp) - 1;
        T[HP].offset[0] = 1 - static_cast<GLfloat>(hp) / static_cast<GLfloat>(max_hp);
    };
    std::pair<float, Motion::FunType> _stand = {1.0, stand};
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


void Unit::Update() {
    static float& flip = *motion.FindUniformFloat("flip"); 
    static int& hp = *motion.FindUniformInt("HP"); 
    Vector3<GLfloat> hp_color;
    hp = params.HP;

    flip = fmod(args.deg, 360);
    Actor::SetSpriteColor(this, std::string("circle_bone"), color);
    if (params.HP >= 50) {
        hp_color = Vector3<GLfloat>(static_cast<GLfloat>(max_hp - params.HP) / max_hp * 2, 1.0, 0.0);
    } else {
        hp_color = Vector3<GLfloat>(1.0, static_cast<GLfloat>(params.HP) / max_hp * 2, 0.0);
    }
    Actor::SetSpriteColor(this, std::string("hp"), hp_color);
}

void Unit::SetID(int id_) {
    id = id_;
    if (id < 0 || id > 5) {
        std::cerr << "Invalid player ID: " << id << std::endl;
        color = Vector3<GLfloat>(1.0, 1.0, 1.0);
    }

    GLfloat r = 0.0;
    GLfloat g = 0.0;
    GLfloat b = 0.0;

    switch (id / 3)
    {
    case 0:
        r = 1.0;
        if (id % 3 == 1) {
            g = 1.0;
        } else if (id % 3 == 2) {
            b = 1.0;
        }
        break;
    case 1:
        if (id % 3 == 2) {
            b = 1.0;
        } else {
            g = 1.0;
            if (id % 3 == 1) {
                b = 1.0;
            }
        }

        break;
    }

    color = Vector3<GLfloat>(r, g, b);
}


Bullet* Unit::Fire() {
    if (!isAlive() || (Actor::GetTime() - last_fire < (1e9 / rate_fire))) return nullptr;
    Bullet* bullet = new Bullet();
    bullet->updateAnimation("stand");
    bullet->SetDirection(Vector3<GLfloat>(-cos(ToRadian(args.deg)), -sin(ToRadian(args.deg)), 0.0));
    bullet->Teleport(GetTransform()->GetWorldPos());
    bullet->MoveForward(1.0);
    bullet->color = color;
    bullet->owner = id;
    last_fire = Actor::GetTime();

    return bullet;
}

bool Unit::isAlive() {
    return params.HP > 0;
}

void Unit::DealingDamage(int damage) {
    params.HP -= damage;
    if (!isAlive()) updateAnimation("kill");
}
