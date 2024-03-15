#pragma once

#include "../object/sprite.hpp"
#include "../entities/components/bone.hpp"
#include <filesystem>
#include <pugixml.hpp>

namespace fs = std::filesystem;

template <typename Derived>
class Actor 
{
public:
    Actor(const std::string &path)
    {
        loadActor(path);
        trans.Rotate = Vector3<GLfloat>(0.0, 0.0, 180);
    }
    ~Actor()
    {
        delete[] components;
    }


    std::vector<Component*> getActorComponents(Bone *_parent, size_t &n)
    {
        std::vector<Component*> ActorComponents;
        size_t parentNumber = n++;

        for (auto it : _parent->children) {
            objectTransform *component       = &it->animation.transform;
            objectTransform *ParentComponent = &_parent->animation.transform;
            objectTransform *ParentSprite    = &components[parentNumber].transform;

            components[n].transform.WorldPos = component->WorldPos + ParentSprite->WorldPos;
            components[n].transform.Rotate   = component->Rotate   + ParentSprite->Rotate;
            components[n].transform.Scale    = component->Scale    * ParentComponent->Scale * it->animation.spriteScale;

            components[n].sprite = it->animation.sprite;
            ActorComponents.push_back(&components[n]);
            // std::cout << "N=" << n << '\t' << "Sprite=" << components[n].sprite->name << std::endl;
            
            std::vector<Component*> componentsToAdd = getActorComponents(it, n);
            ActorComponents.insert(ActorComponents.end(), componentsToAdd.begin(), componentsToAdd.end());
        }

        return ActorComponents;
    }

    std::vector<Component*> getActorComponents()
    {
        components[0].transform = trans; // Updating the skeleton position
        size_t n = 0;
        if (components == nullptr) {
            std::vector<Component*> ActorComponents;
            return ActorComponents;
        }
        return getActorComponents(&Derived::skelet, n);
    }

    void updateAnimation()
    {
        // for (auto it : components) {
        // }
    }


    static void parseAnimation(pugi::xml_node &_node, Bone *_bone) {
        for (int i = 0; i < _bone->children.size(); i++) {
            pugi::xml_node node = _node.child(_bone->children[i]->name.c_str());

            std::string spriteName = node.attribute("sprite").as_string();
            auto it = Derived::Sprites.find(spriteName); 
            if (it != Sprites.end()) {
                _bone->children[i]->animation.sprite = &(it->second); 
                _bone->children[i]->animation.spriteScale = it->second.Scale; 
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

            _bone->children[i]->animation.transform.SetTransform(_transform);

            parseAnimation(node, _bone->children[i]);
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

        // Derived::skelet.animation.transform.SetTransform(_transform);
        // Derived::skelet.animation.component.transform = Derived::skelet.animation.transform;

        parseAnimation(node, &Derived::skelet);

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

protected:
    std::string name;
    objectTransform trans;
    Component *components = nullptr;
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
