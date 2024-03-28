#include <entities/actor.hpp>
#include <entities/templates/mobs/spider.hpp>

std::map<std::string, Sprite> Spider::Sprites;
size_t Spider::skeletSize = 0;
Bone Spider::skelet;

Spider::Spider() : Character(std::string("mobs/spider"))
{
    name = "Spider";

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

Spider::~Spider()
{}

void Spider::Initialize()
{
    std::string path("mobs/spider");
    name = "spider";
    std::vector<std::string> _animations = {"stand"};
    Actor::Initialize<Spider>(path, name, _animations);


    // std::string path("mobs/spider");
    // name = "spider";
    // loadSkelet(path);
    // loadSprites(path);
    // loadAnimation(path, "stand");

    std::cout << "Spider skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
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