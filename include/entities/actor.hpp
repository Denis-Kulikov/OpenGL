#pragma once

#include "../object/sprite.hpp"
#include "../entities/components/bone.hpp"
// #include "../game/gameTime.hpp"
#include <filesystem>
#include <pugixml.hpp>

#define MY_ACTOR_TEST true

#if MY_ACTOR_TEST
// для отладки
#include "../render/render.hpp"
#include "../object/sphere.hpp"
#include "../object/line.hpp"
#endif


// extern GameTime Time;

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


template <typename Derived>
class Actor 
{
public:
    Actor(const std::string &path)
    {
        loadActor(path);
        trans.Rotate = Vector3<GLfloat>(0.0, 0.0, 180);
        direction = Vector3<GLfloat>();

        globalFlip = new GLfloat[Derived::skeletSize];
        components = new Component[Derived::skeletSize];
        animations = new Animation*[Derived::skeletSize];
        for (int i = 0; i < Derived::skeletSize; i++) {
            animations[i] = nullptr;
            globalFlip[i] = 0;
        }
        animations[0] = new Animation();

        #if MY_ACTOR_TEST
        spherePos = new Vector3<GLfloat>[Derived::skeletSize]();

        mySphere = sphere(std::string("mySphere"), "shaders/sphere_fs.glsl", "shaders/sphere_vs.glsl", nullptr, 10);
        sphereTransform.Rotate = Vector3<GLfloat>(0.0, 0.0, 0.0);
        sphereTransform.Scale = Vector3<GLfloat>(0.1, 0.2, 0.1);
        
        myLine = line(std::string("myLine"), Vector3<GLfloat>(1.0, 0.0, 0.0));
        #endif
    }
    
    ~Actor()
    {
        delete[] components;
        delete[] animations;
    }


    std::vector<Component*> getActorComponents(Bone *_parent, size_t &n)
    {
        std::vector<Component*> ActorComponents;
        size_t parentNumber = n++;

        for (const auto &it : _parent->children) {
            const Vector3<GLfloat> &anchorPoint    = animations[n]->anchorPoint;
            const objectTransform &component       = animations[n]->transform;
            const objectTransform &ParentComponent = animations[parentNumber]->transform;
            const objectTransform &ParentSprite    = components[parentNumber].transform;
            // GLfloat flipMotion = component.Rotate.x;
            GLfloat flipMotion = component.Rotate.x + animations[n]->motion.GetFlip(AnimationTimeStart);
            globalFlip[n] = globalFlip[parentNumber] + flipMotion;
            GLfloat parentFlipAngle = globalFlip[parentNumber];
            GLfloat flipAngle = globalFlip[n];


            components[n].transform.WorldPos.x = ParentSprite.WorldPos.x; // не адаптируется положение от размера старших костей 
            components[n].transform.WorldPos.y = ParentSprite.WorldPos.y;
            components[n].transform.WorldPos.z = ParentSprite.WorldPos.z + component.WorldPos.z;

            components[n].transform.Rotate.y = component.Rotate.y + ParentSprite.Rotate.y;
            components[n].transform.Rotate.z = component.Rotate.z + ParentSprite.Rotate.z + flipMotion;

            components[n].transform.Scale.x = component.Scale.x * animations[n]->spriteScale.x * ParentSprite.Scale.x / animations[parentNumber]->spriteScale.x;
            components[n].transform.Scale.y = component.Scale.y * animations[n]->spriteScale.y * ParentSprite.Scale.y / animations[parentNumber]->spriteScale.y;


            Vector3<GLfloat> direction = Vector3<GLfloat>(cos(ToRadian(parentFlipAngle)), sin(ToRadian(parentFlipAngle)), 0.0f);
            components[n].transform.Move(component.WorldPos.x * animations[parentNumber]->spriteScale.x, direction);
            parentFlipAngle += 90;
            direction = Vector3<GLfloat>(cos(ToRadian(parentFlipAngle)), sin(ToRadian(parentFlipAngle)), 0.0f);
            components[n].transform.Move(component.WorldPos.y * animations[parentNumber]->spriteScale.y, direction);

            #if MY_ACTOR_TEST
            render->PushGeometry(mySphere.GetGeometry());
            sphereTransform.WorldPos = components[n].transform.WorldPos;
            spherePos[n] = sphereTransform.WorldPos;
            if (render != nullptr) render->drawObject(&sphereTransform, &mySphere);
            render->PushGeometry(myLine.GetGeometry());
            myLine.setPoints(spherePos[n], spherePos[parentNumber]);
            if (render != nullptr) render->drawObject(&myLine.trans, &myLine);
            spherePos[0] = spherePos[1];
            render->PushGeometry(&Sprite::geometryInfo);
            #endif

            direction = Vector3<GLfloat>(cos(ToRadian(flipAngle)), sin(ToRadian(flipAngle)), 0.0f);
            components[n].transform.Move(-anchorPoint.x, direction);
            flipAngle += 90;
            direction = Vector3<GLfloat>(cos(ToRadian(flipAngle)), sin(ToRadian(flipAngle)), 0.0f);
            components[n].transform.Move(-anchorPoint.y, direction);

            ActorComponents.push_back(&components[n]);
            std::vector<Component*> componentsToAdd = getActorComponents(it, n);
            ActorComponents.insert(ActorComponents.end(), componentsToAdd.begin(), componentsToAdd.end());
        }

        return ActorComponents;
    }

    std::vector<Component*> getActorComponents()
    {
        if (components == nullptr) {
            std::vector<Component*> ActorComponents;
            return ActorComponents;
        }
        
        #if MY_ACTOR_TEST
        using Clock = std::chrono::steady_clock;
        using TimePoint = std::chrono::time_point<Clock>;
        TimePoint current = Clock::now();
        auto currentTimeMilliseconds = std::chrono::time_point_cast<std::chrono::milliseconds>(current).time_since_epoch().count();

        if (name == "Wilson") {
            animations[5]->transform.Rotate.x = 100 + sin(currentTimeMilliseconds / 1000.0) * 30.0; 
            animations[6]->transform.Rotate.x = -20 + sin(currentTimeMilliseconds / 1000.0) * 10.0; 
            animations[7]->transform.Rotate.x = -10 + sin(currentTimeMilliseconds / 1000.0) * 20.0; 
            // animations[2]->transform.Rotate.x = sin(currentTimeMilliseconds / 1000.0) * 10; 
        }
        #endif
        
        size_t n = 0;
        animations[0]->transform = components[0].transform = trans; // Updating the skelet position

        return getActorComponents(&Derived::skelet, n);
    }


    void updateAnimationRecursive(Bone *_parent, const std::string &animationName, size_t &n)
    {
        n++;
        for (auto it : _parent->children) {
            if (it->Animations.find(animationName) != it->Animations.end()) {
                animations[n] = &it->Animations[animationName];
                components[n].sprite = animations[n]->sprite; 
            }

            updateAnimationRecursive(it, animationName, n);
        }
    }

    void updateAnimation(const std::string &animationName)
    {
        if (animation == animationName) return;
        if (Derived::skelet.Animations.find(animationName) != Derived::skelet.Animations.end()) {
            animation = animationName;
            animations[0] = &Derived::skelet.Animations[animationName];
        }

        size_t n = 0;
        updateAnimationRecursive(&Derived::skelet, animationName, n);
    }


    static void parseAnimation(pugi::xml_node &_node, Bone *_bone, const std::string &animationName) {
        for (int i = 0; i < _bone->children.size(); i++) {
            Animation newAnimation;
            pugi::xml_node node = _node.child(_bone->children[i]->name.c_str());

            std::string spriteName = node.attribute("sprite").as_string();
            auto sprite = Derived::Sprites.find(spriteName); 
            if (sprite != Sprites.end()) {
                newAnimation.sprite = &(sprite->second); 
                newAnimation.spriteScale = sprite->second.Scale; 
            } else {
                std::cout << "Error Actor.parseAnimation: not found " << spriteName << std::endl;
            }

            objectTransform _transform;
            Vector3<GLfloat> v;

            v.x = std::stof(node.attribute("x").value());
            v.y = std::stof(node.attribute("y").value());
            v.z = std::stof(node.attribute("z").value()) / 10;
            _transform.WorldPos = v;

            v.x = std::stof(node.attribute("width").value());
            v.y = std::stof(node.attribute("height").value());
            v.z = 0.0;
            _transform.Scale = v; 

            v.x = std::stof(node.attribute("flip").value());
            v.y = 180.0 * (node.attribute("mirrorX") != 0);
            v.z = std::stof(node.attribute("rotate").value());
            _transform.Rotate = v;

            newAnimation.transform = _transform;

            v.x = std::stof(node.attribute("tangent").value());
            v.y = std::stof(node.attribute("radius").value());
            v.z = 0.0;
            newAnimation.anchorPoint = v;

            _bone->children[i]->Animations.insert({animationName, newAnimation});
            parseAnimation(node, _bone->children[i], animationName);
        }
    }

    static bool loadAnimation(const std::string &_path, const std::string &_name)
    {
        std::string full_path = std::string("assets/entities/") + _path + std::string("/models/animations/") + _name + std::string(".xml");
        pugi::xml_document doc;
        pugi::xml_node node;
        pugi::xml_parse_result parse_result = doc.load_file(full_path.c_str());

        if (!parse_result) {
            std::cout << "Error Actor.loadAnimation: file not found (" << full_path << ")" << std::endl;
            return false;
        }

        node = doc.child("animation");
        std::string animationName = node.attribute("name").as_string();
        
        objectTransform _transform;
        Vector3<GLfloat> v;

        v.x = std::stof(node.attribute("x").value());
        v.y = std::stof(node.attribute("y").value());
        v.z = std::stof(node.attribute("z").value());
        _transform.SetWorldPos(v.x, v.y, v.z);

        v.x = std::stof(node.attribute("width").value());
        v.y = std::stof(node.attribute("height").value());
        _transform.SetScale(v.x, v.y, 0.0);
        
        v.z = std::stof(node.attribute("flip").value());
        _transform.SetRotate(0.0, 0.0, v.z);

        if (node.attribute("mirrorX")) {
            _transform.SetRotate(0.0, 180.0, v.z);
        } 

        Animation newAnimation;
        newAnimation.sprite = nullptr;
        newAnimation.spriteScale = Vector3<GLfloat>(1.0, 1.0, 1.0);
        newAnimation.transform = objectTransform();
        Derived::skelet.Animations.insert({animationName, newAnimation});

        parseAnimation(node, &Derived::skelet, animationName);

        return true;
    }

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

    static bool loadSkelet(const std::string &path)
    {
        Derived::skeletSize = Derived::skelet.createSkelet(path, "skelet");

        return true;
    }

    bool loadActor(const std::string &path)
    {
        std::string full_path = std::string("assets/entities/") + path + "/actor.xml";
        pugi::xml_document doc;
        pugi::xml_node word;
        pugi::xml_parse_result parse_result = doc.load_file(full_path.c_str());

        if (!parse_result) {
            std::cout << "Error " << name << ".loadActor: file not found (" << full_path << ")" << std::endl;
            return false;
        }

        name = doc.child("character").attribute("name").value();

        Vector3<GLfloat> v;

        v.x = std::stof(doc.child("character").child("objectTransform").child("worldPos").attribute("x").value());
        v.y = std::stof(doc.child("character").child("objectTransform").child("worldPos").attribute("y").value());
        v.z = std::stof(doc.child("character").child("objectTransform").child("worldPos").attribute("z").value());
        trans.SetWorldPos(v.x, v.y, v.z);

        v.x = std::stof(doc.child("character").child("objectTransform").child("rotate").attribute("deg").value());
        trans.SetRotate(0.0, 0.0, v.x);

        v.x = std::stof(doc.child("character").child("objectTransform").child("scale").attribute("x").value());
        v.y = std::stof(doc.child("character").child("objectTransform").child("scale").attribute("y").value());
        trans.SetScale(v.x, v.y, 0.0);

        return true;
    }


    objectTransform *GetTransform()
    {
        return &trans;
    }

    Vector3<GLfloat> GetDirection()
    {
        return direction;
    }

    std::string GetAnimationByAction()
    {
        switch (state) {
        case 0:
            return std::string("stand");
        default:
            return std::string("stand");
        }
    }

    std::string GetAnimation(const float CurrentTime)
    {
        switch (state) {
        case STATE::STAND:
            if (direction.Length() != 0) {
                SetState(STATE::GO, CurrentTime);
            }
            return std::string("stand");

        case STATE::GO: 
            if (direction.Length() == 0) {
                SetState(STATE::STAND, CurrentTime);
            }
            return std::string("stand_2");

        case STATE::ACTION:
            return GetAnimationByAction();

        case STATE::GET_HIT:
            return std::string("stand");

        default:
            return std::string("stand");
        }
    }

    void SetState(int _state, const float CurrentTime)
    {
        state = _state;
        AnimationTimeStart = CurrentTime;
    }

    static void PushTime(const float _time)
    {
        Motion::PushTime(_time);
    }

    

    // для отладки
    #if MY_ACTOR_TEST
    void PushRender(Render *_render)
    {
        render = _render;
    }

    Render *render = nullptr;
    Vector3<GLfloat> *spherePos = nullptr;
    objectTransform sphereTransform;
    sphere mySphere;
    line myLine;
    #endif

    Vector3<GLfloat> direction;
    std::string name = "NoName";

protected:
    float AnimationTimeStart = 0.0;
    GLfloat *globalFlip = nullptr;
    Component *components = nullptr;
    Animation **animations = nullptr;
    objectTransform trans;
    std::string animation;
    int state = STATE::STAND;
    static size_t skeletSize; // add inline
    static Bone skelet;
    static std::map<std::string, Sprite> Sprites;
};

template <typename Derived>
std::map<std::string, Sprite> Actor<Derived>::Sprites;

template <typename Derived>
Bone Actor<Derived>::skelet;

template <typename Derived>
size_t Actor<Derived>::skeletSize;
