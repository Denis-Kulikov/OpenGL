#include <entities/actor.hpp>
#include <filesystem>

namespace fs = std::filesystem;

std::map<std::string, Sprite> Actor::sprites;

// std::map<std::string, Bone> Bone::children;
// std::map<Sprite, std::string> Actor::sprites(Sprite(), "");

// std::vector<Sprite*> Actor::sortSpritesZ(std::vector<Sprite*> &sprites)
// {
//     for (auto it : sprites) {

//     }

//     return sprites
// }

std::vector<Sprite*> Actor::getActorComponents(Bone *_parent)
{
    std::vector<Sprite*> ActorComponents;

    for (auto it : _parent->children) {
        // std::cout << __FUNCTION__ << " " << it->name << std::endl;

        objectTransform *transChild  = &(it->component.trans);
        objectTransform *transParent;

        // if (it->parent != nullptr) {
            transParent = &(it->parent->component.trans);
            // transParent->print();
        // } else {
            // transParent = new objectTransform();
            // transParent->SetScale(1.0, 1.0, 1.0);
        // }

        if (it->name == "head") {
            transParent->Move(0.0, 0.0, 0.0);
            transParent->SetScale(1.0, 1.0, 0);
        }
        if (it->name == "face") {
            transParent->Move(0.0, 0.0, -0.1);
            transParent->SetScale(0.6, 0.6, 0);
        }
        if (it->name == "hair_front") {
            transParent->Move(0.0, 0.0, 0.1);
            // transParent->Move(0.0, 0.5, 0.1);
            transParent->SetScale(1.5, 1.5, 0);
        }

        it->component.sprite->trans.SetWorldPos(transChild->WorldPos.x + transParent->WorldPos.x, transChild->WorldPos.y + transParent->WorldPos.y, transChild->WorldPos.z + transParent->WorldPos.z);
        it->component.sprite->trans.SetRotate  (transChild->Rotate.x   + transParent->Rotate.x,   transChild->Rotate.y   + transParent->Rotate.y,   transChild->Rotate.z   + transParent->Rotate.z);
        it->component.sprite->trans.SetScale   (transChild->Scale.x    * transParent->Scale.x,    transChild->Scale.y    * transParent->Scale.y,    transChild->Scale.z    * transParent->Scale.z);
        
        ActorComponents.push_back(it->component.sprite);
        
        std::vector<Sprite*> componentsToAdd = getActorComponents(it);
        ActorComponents.insert(ActorComponents.end(), componentsToAdd.begin(), componentsToAdd.end());
    }

    // vector<pair<int, double>> sortedVector(docs->begin(), docs->end());
    // sort(ActorComponents.begin(), ActorComponents.end(), [](const auto& lhs, const auto& rhs) { return sprite.trans->WorldPos.z > sprite.trans->WorldPos.z;});

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

    float n = std::stoi(doc.child("components").attribute("n").value());
    std::cout << "N: " << n << std::endl;
    

    return true;
}

// std::set<Sprite, std::string> Actor::sprites; загружать спрайты в эту структуру
bool Actor::loadSprites(const std::string &path)
{
    std::cout << __FUNCTION__  << std::endl;
    std::string full_path = "entities/" + path + "/models/sprites/";

    for (const auto &entry : fs::directory_iterator("assets/" + full_path)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if ((filename.size() > 4) && (filename.substr(filename.size() - 4) == ".jpg" || filename.substr(filename.size() - 4) == ".png")) {
                std::string spriteName = filename.substr(0, filename.size() - 4);
                std::cout << spriteName  << std::endl;
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
    // std::string full_path("assets/entities/player/Wilson/actor.xml");
    pugi::xml_document doc;
    pugi::xml_node word;
    pugi::xml_parse_result parse_result = doc.load_file(full_path.c_str());
    // pugi::xml_parse_result parse_result = doc.load_file("assets/entities/player/Wilson/actor.xml");

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

    std::cout << "Name: " << name << std::endl;
    trans.print();

    if (!loadComponents(path)) return false;

    //  = stoi(doc.child("fts").child("parser").attribute("ngram_max_length").value());
    // word = doc.child("fts").child("parser").child("stop_words").child("word");
    // while (static_cast<bool>(word)) {
    //     stop_words.emplace_back(word.child_value());
    //     word = word.next_sibling("word");
    // }

    return true;
}


// Actor::Actor(const std::string &_name, const objectTransform &_trans)
//     : name(_name), trans(_trans)
// {

// }

Actor::Actor(const std::string &path)
{
    loadActor(path);
    skelets.component.trans.SetRotate(0.0, 0.0, 180);

    // skelets.pushSprites(&sprites);
}