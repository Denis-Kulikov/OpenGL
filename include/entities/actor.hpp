#pragma once

#include "../object/sprite.hpp"
#include "../entities/components/bone.hpp"
#include <filesystem>
#include <pugixml.hpp>

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

        for (auto it : _parent->children) {
            objectTransform *component       = &animations[n]->transform;
            objectTransform *ParentComponent = &animations[parentNumber]->transform;
            objectTransform *ParentSprite    = &components[parentNumber].transform;

            if (ParentComponent == nullptr) {
                std::cout << "parentNumber: " << parentNumber << std::endl;
                return ActorComponents;
            }

            components[n].transform.WorldPos = component->WorldPos + ParentSprite->WorldPos;
            components[n].transform.Rotate   = component->Rotate   + ParentSprite->Rotate;
            components[n].transform.Scale    = component->Scale    * ParentComponent->Scale * animations[n]->spriteScale; // изменить зависимость от родительских костей 

            ActorComponents.push_back(&components[n]);
            
            std::vector<Component*> componentsToAdd = getActorComponents(it, n);
            ActorComponents.insert(ActorComponents.end(), componentsToAdd.begin(), componentsToAdd.end());
        }

        return ActorComponents;
    }

    std::vector<Component*> getActorComponents()
    {
        animations[0]->transform = components[0].transform = trans; // Updating the skelet position
        size_t n = 0;
        if (components == nullptr) {
            std::vector<Component*> ActorComponents;
            return ActorComponents;
        }
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
            // animations[0] = &Derived::skelet.Animations[animationName];
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
            v.z = std::stof(node.attribute("z").value());
            _transform.SetWorldPos(v.x, v.y, v.z / 10);

            v.x = std::stof(node.attribute("width").value());
            v.y = std::stof(node.attribute("height").value());
            _transform.SetScale(v.x, v.y, 0.0);

            v.z = std::stof(node.attribute("flip").value());
            _transform.SetRotate(0.0, 0.0, v.z);

            if (node.attribute("mirrorX")) {
                _transform.SetRotate(0.0, 180.0, v.z);
            } 

            newAnimation.transform = _transform;
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

    std::string GetAnimation()
    {
        switch (state) {
        case STATE::STAND:
            if (direction.Length() != 0) {
                SetState(STATE::GO);
            }
            return std::string("stand");

        case STATE::GO: 
            if (direction.Length() == 0) {
                SetState(STATE::STAND);
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

    void SetState(int _state)
    {
        state = _state;
    }

    Vector3<GLfloat> direction;

protected:
    std::string name;
    objectTransform trans;
    Component *components = nullptr;
    Animation **animations = nullptr;
    std::string animation;
    int state = STATE::STAND;
    static size_t skeletSize;
    static Bone skelet;
    static std::map<std::string, Sprite> Sprites;
};

template <typename Derived>
std::map<std::string, Sprite> Actor<Derived>::Sprites;

template <typename Derived>
Bone Actor<Derived>::skelet;

template <typename Derived>
size_t Actor<Derived>::skeletSize;
