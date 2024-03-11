#include <entities/actor.hpp>
#include <entities/components/component.hpp>
#include <filesystem>
#include <pugixml.hpp>

namespace fs = std::filesystem;

// Bone Actor::skelet;
std::map<std::string, Sprite> Actor::Sprites;
const struct NODE_STR Actor::NODE = {"name", "bone"};


void printBones(Bone *_bone)
{
    std::cout << std::endl;
    std::cout << _bone->name << std::endl;
    if (_bone->animation.sprite != nullptr) _bone->animation.sprite->trans.print();
    for (auto it : _bone->children) printBones(it);
}

// нужно убрать tran у sprite и добавить его в components
std::vector<Component*> Actor::getActorComponents(Bone *_parent)
{
    std::vector<Component*> ActorComponents;
    skelet.animation.component.transform = trans;

    for (auto it : _parent->children) {
        objectTransform *transChild  = &(it->animation.trans);
        objectTransform *transParentComponent = &_parent->animation.trans;
        objectTransform *transParentSprite = &_parent->animation.component.transform;

        it->animation.component.transform.WorldPos  = transChild->WorldPos + transParentSprite->WorldPos;
        it->animation.component.transform.Rotate    = transChild->Rotate + transParentSprite->Rotate;
        it->animation.component.transform.Scale     = transChild->Scale * it->animation.spriteScale * transParentComponent->Scale;

        it->animation.component.sprite = it->animation.sprite;
        // it->animation.component.transform.print();
        ActorComponents.push_back(&it->animation.component);
        
        std::vector<Component*> componentsToAdd = getActorComponents(it);
        ActorComponents.insert(ActorComponents.end(), componentsToAdd.begin(), componentsToAdd.end());
    }

    return ActorComponents;
}

void Actor::parseAnimation(pugi::xml_node &_node, Bone *_bone) {
    for (int i = 0; i < _bone->children.size(); i++) {
        pugi::xml_node node = _node.child(_bone->children[i]->name.c_str());

        std::string spriteName = node.attribute("sprite").as_string();
        auto it = Sprites.find(spriteName); 
        if (it != Sprites.end()) { // try
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

        _bone->children[i]->animation.trans.SetTransform(_transform);

        parseAnimation(node, _bone->children[i]);
    }
}

bool Actor::loadAnimation(const std::string &_path, const std::string &_name)
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
    // skelet.animation.name = node.attribute(Actor::NODE.NAME).value();

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

    skelet.animation.trans.SetTransform(_transform);
    skelet.animation.component.transform = skelet.animation.trans;

    parseAnimation(node, &skelet);

    // printBones(&skelet);

    return true;
}

bool Actor::loadSprites(const std::string &path)
{
    std::string full_path = "entities/" + path + "/models/sprites/";

    for (const auto &entry : fs::directory_iterator("assets/" + full_path)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if ((filename.size() > 4) && (filename.substr(filename.size() - 4) == ".jpg" || filename.substr(filename.size() - 4) == ".png")) {
                std::string spriteName = filename.substr(0, filename.size() - 4);
                Sprite sprite(spriteName, "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", (full_path + filename).c_str());
                Sprites.insert({spriteName, sprite});
            }
        }
    }

    return true;
}

bool Actor::loadActor(const std::string &path)
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

// Actor::Actor(const std::string &_name, const objectTransform &_trans)
//     : name(_name), trans(_trans)
// {

// }

Actor::Actor(const std::string &path)
{
    loadActor(path);
    sprites = &Sprites;
    // skelet = &skelet;
    // skelet.animation.trans = trans;
    trans.Rotate = Vector3<GLfloat>(0.0, 0.0, 180);
}