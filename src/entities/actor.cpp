#include <entities/actor.hpp>
#include <entities/components/components.hpp>
#include <filesystem>
#include <pugixml.hpp>

namespace fs = std::filesystem;

std::map<std::string, Sprite> Actor::sprites;
const struct NODE_STR Actor::NODE = {"name", "bone"};

void Actor::parseAnimation(pugi::xml_node &_node, Bone *_bone) {
    for (int i = 0; i < _bone->children.size(); i++) {
        pugi::xml_node node = _node.child(_bone->children[i]->name.c_str());
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

        _bone->children[i]->component.SetChildAnimation(_transform);
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
        std::cout << "Error " << name << ".loadAnimation: file not found (" << full_path << ")" << std::endl;
        return false;
    }

    node = doc.child("animation");
    skelet.component.name = node.attribute(Actor::NODE.NAME).value();
    
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

    skelet.component.trans.SetTransform(_transform);

    parseAnimation(node, &skelet);

    return true;
}

std::vector<Sprite*> Actor::getActorComponents(Bone *_parent)
{
    std::vector<Sprite*> ActorComponents;

    for (auto it : _parent->children) {
        objectTransform *transChild  = &(it->component.trans);
        objectTransform *transParent = &(it->parent->component.trans);

        it->component.sprite->trans.SetWorldPos(transChild->WorldPos.x + transParent->WorldPos.x, transChild->WorldPos.y + transParent->WorldPos.y, transChild->WorldPos.z + transParent->WorldPos.z);
        it->component.sprite->trans.SetRotate  (transChild->Rotate.x   + transParent->Rotate.x,   transChild->Rotate.y   + transParent->Rotate.y,   transChild->Rotate.z   + transParent->Rotate.z);
        it->component.sprite->trans.SetScale   (transChild->Scale.x    * transParent->Scale.x,    transChild->Scale.y    * transParent->Scale.y,    transChild->Scale.z    * transParent->Scale.z);
        
        ActorComponents.push_back(it->component.sprite);
        
        std::vector<Sprite*> componentsToAdd = getActorComponents(it);
        ActorComponents.insert(ActorComponents.end(), componentsToAdd.begin(), componentsToAdd.end());
    }

    return ActorComponents;
}

bool Actor::loadComponents(const std::string &path)
{
    std::string full_path = std::string("assets/entities/") + path + "/models/components.xml";
    pugi::xml_document doc;
    pugi::xml_node word;
    pugi::xml_parse_result parse_result = doc.load_file(full_path.c_str());

    if (!parse_result) {
        std::cout << "Error " << name << ".loadComponents: file not found (" << full_path << ")" << std::endl;
        return false;
    }

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
                sprites.insert({spriteName, sprite});
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

    if (!loadComponents(path)) return false;

    return true;
}

// Actor::Actor(const std::string &_name, const objectTransform &_trans)
//     : name(_name), trans(_trans)
// {

// }

Actor::Actor(const std::string &path)
{
    loadActor(path);
    skelet.component.trans.SetRotate(0.0, 0.0, 180);

    // skelet.pushSprites(&sprites);
}