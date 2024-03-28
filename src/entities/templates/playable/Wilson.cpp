#include <entities/actor.hpp>
#include <entities/templates/playable/Wilson.hpp>

std::map<std::string, Sprite> Wilson::Sprites;
Bone Wilson::skelet;
size_t Wilson::skeletSize;

Wilson::Wilson()
    : Character(std::string("player/Wilson"))
{
    name = "Wilson";

        globalFlip = new GLfloat[GetSkeletSize()];
        components = new Component[GetSkeletSize()];
        animations = new Animation*[GetSkeletSize()];
        for (int i = 0; i < GetSkeletSize(); i++) {
            animations[i] = nullptr;
        }
        animations[0] = new Animation();
        globalFlip[0] = 0;


        #if MY_ACTOR_TEST
        spherePos = new Vector3<GLfloat>[GetSkeletSize()]();

        mySphere = sphere(std::string("mySphere"), "shaders/sphere_fs.glsl", "shaders/sphere_vs.glsl", nullptr, 10);
        sphereTransform.Rotate = Vector3<GLfloat>(0.0, 0.0, 0.0);
        sphereTransform.Scale = Vector3<GLfloat>(0.1, 0.2, 0.1);
        
        myLine = line(std::string("myLine"), Vector3<GLfloat>(1.0, 0.0, 0.0));
        #endif
}

Wilson::~Wilson()
{
    std::cout << "Wilson::~Wilson()" << std::endl;
}

void Wilson::Initialize()
{
    std::string path("player/Wilson");
    name = "Wilson";
    std::vector<std::string> _animations = {"stand", "stand_2"};
    Actor::Initialize<Wilson>(path, name, _animations);

    // loadSkelet(path);
    // loadSprites(path);
    // loadAnimation(path, "stand");

    
    // loadAnimation(path, "stand_2");

    std::cout << "Wilson skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
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

// вектор указателей

// вариант