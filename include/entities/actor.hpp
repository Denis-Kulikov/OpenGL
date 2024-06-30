#pragma once

#include "../object/sprite.hpp"
#include "../entities/components/motion.hpp"
#include "../entities/components/animationInfo.hpp"
#include <filesystem>
// #include "pugixml.hpp"

#define MY_ACTOR_TEST false

#if MY_ACTOR_TEST
#include "../render/render.hpp"
#include "../object/sphere.hpp"
#include "../object/line.hpp"
#endif


namespace fs = std::filesystem;

namespace STATE
{
    enum
    {
        DEFAULT,
        STAND,
        GO,
        ACTION,
        GET_HIT
    };
}

constexpr size_t myHash(const char* s)
{
    size_t hash = 0;
    while (*s) {
        hash = (hash * 31) + *s;
        ++s;
    }
    
    return hash;
}

class Actor 
{
public:
    Actor(const std::string &path, const size_t SkeletSize);
    
    ~Actor();

    virtual size_t GetSkeletSize() const = 0;
    virtual Bone *GetSkelet() const = 0;
    virtual std::map<std::string, Sprite> *GetSprites() const = 0;
    virtual std::string *GetName() const = 0;

    std::vector<Component*> getActorComponents(const Bone &_parent, size_t &n, const GLfloat duration);
    std::vector<Component*> getActorComponents();
    void updateAnimationRecursive(Bone &_parent, const std::string &animationName, size_t &n);
    void updateAnimation(const std::string &animationName);

    static void parseNodeAnimation(pugi::xml_node &nodeAnimation, const std::string &animationName, Animation &newAnimation, std::map<std::string, Sprite> &Sprites);

    template<typename Derived>
    static void parseAnimation(pugi::xml_node &nodeAnimation, pugi::xml_node &nodeMotion, Bone &_bone, const std::string &animationName) {
        for (int i = 0; i < _bone.children.size(); i++) {
            Animation newAnimation;
            std::string boneName = _bone.children[i].name;
            pugi::xml_node node = nodeAnimation.child(boneName.c_str());
                
            parseNodeAnimation(node, animationName, newAnimation, Derived::Sprites);
            _bone.children[i].Animations.insert({animationName, newAnimation});

            parseAnimation<Derived>(node, nodeMotion, _bone.children[i], animationName);
        }
    }

    template<typename Derived>
    static bool loadAnimation(const std::string &_path, const std::string &_name)
    {
        std::string animationPath = std::string("assets/entities/") + _path + std::string("/models/animations/") + _name + std::string(".xml");
        pugi::xml_document docAnimation;
        pugi::xml_node nodeAnimation;
        pugi::xml_node nodeMotion;
        pugi::xml_parse_result parseAnimationResult = docAnimation.load_file(animationPath.c_str());

        if (!parseAnimationResult) {
            std::cout << "Error Actor.loadAnimation: file not found (" << animationPath << ")" << std::endl;
            return false;
        }

        nodeAnimation = docAnimation.child("animation");
        std::string animationName = nodeAnimation.attribute("name").as_string();
        
        objectTransform _transform;
        Vector3<GLfloat> v;

        v.x = std::stof(nodeAnimation.attribute("x").value());
        v.y = std::stof(nodeAnimation.attribute("y").value());
        v.z = std::stof(nodeAnimation.attribute("z").value());
        _transform.SetWorldPos(v.x, v.y, v.z);

        v.x = std::stof(nodeAnimation.attribute("width").value());
        v.y = std::stof(nodeAnimation.attribute("height").value());
        _transform.SetScale(v.x, v.y, 0.0);
        
        v.z = std::stof(nodeAnimation.attribute("flip").value());
        _transform.SetRotate(0.0, 0.0, v.z);

        if (nodeAnimation.attribute("mirrorX")) {
            _transform.SetRotate(0.0, 180.0, v.z);
        } 

        Animation newAnimation;
        newAnimation.sprite = nullptr;
        newAnimation.spriteScale = Vector3<GLfloat>(1.0, 1.0, 1.0);
        newAnimation.transform = objectTransform();
        Derived::skelet.Animations.insert({animationName, newAnimation});

        parseAnimation<Derived>(nodeAnimation, nodeMotion, Derived::skelet, animationName);

        return true;
    }

    template<typename Derived>
    static bool loadSprites(const std::string &path)
    {
        std::string full_path = "entities/" + path + "/models/sprites/";

        for (const auto &entry : fs::directory_iterator("assets/" + full_path)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if ((filename.size() > 4) && (filename.substr(filename.size() - 4) == ".jpg" || filename.substr(filename.size() - 4) == ".png")) {
                    std::string spriteName = filename.substr(0, filename.size() - 4);
                    Sprite sprite(spriteName, "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", (full_path + filename).c_str());
                    Derived::Sprites.insert({spriteName, sprite});
                }
            }
        }

        return true;
    }

    template<typename Derived>
    static bool loadSkelet(const std::string &path)
    {
        Derived::skeletSize = Derived::skelet.createSkelet(path, "skelet");

        return true;
    }

    template<typename Derived>
    static void Initialize(std::string path, std::string _name, std::vector<std::string> _animations)
    {
        Derived::name = _name;
        loadSkelet<Derived>(path);
        loadSprites<Derived>(path);
        Derived::SetMotion();
        for (const auto &it : Derived::motion.function)
            loadAnimation<Derived>(path, it.first);
    }

    bool loadActor(const std::string &path);


    objectTransform *GetTransform();
    Vector3<GLfloat> GetDirection() const;
    void SetDirection(const Vector3<GLfloat> &_direction);
    std::string GetAnimationByAction();
    std::string GetAnimation(const float CurrentTime);

    void SetState(int _state, const float CurrentTime);
    static void PushTime(const float _time);

    float GetTime() const;

    #if MY_ACTOR_TEST
    void PushRender(Render *_render);

    Render *render = nullptr;
    Vector3<GLfloat> *spherePos = nullptr;
    objectTransform sphereTransform;
    sphere mySphere;
    line myLine;
    #endif

protected:
    int state = STATE::STAND;
    inline static float time = 0.0;
    Motion::FunType *motionFunPtr = nullptr;
    Motion *motionPtr = nullptr;
    Vector3<GLfloat> direction = 0;
    objectTransform trans;
    AnimationInfo animationInfo;

    inline static size_t skeletSize = 0;
    inline static Bone skelet; // можно сделать статический массив костей и новую переменную хранящую структуру
    inline static std::string name = "NoName";
    inline static std::map<std::string, Sprite> Sprites;
    inline static Motion motion;
};
