#include <entities/actor.hpp>
#include <filesystem>

namespace fs = std::filesystem;

std::map<std::string, Sprite> Actor::sprites;
// std::map<std::string, Bone> Bone::children;
// std::map<Sprite, std::string> Actor::sprites(Sprite(), "");

bool Actor::loadComponents(const std::string &path)
{
    std::string full_path = std::string("assets/entities/") + path + "/models/components.xml";
    pugi::xml_document doc;
    pugi::xml_node word;
    pugi::xml_parse_result parse_result = doc.load_file(full_path.c_str());

    // word = doc.child("fts").child("parser").child("stop_words").child("word");
    // while (static_cast<bool>(word)) {
    //     stop_words.emplace_back(word.child_value());
    //     word = word.next_sibling("word");
    // }
    if (!parse_result) {
        std::cout << "Error " << name << ".loadComponents: file not found (" << full_path << ")" << std::endl;
        return false;
    }

    float n = std::stoi(doc.child("components").attribute("n").value());
    std::cout << "N: " << n << std::endl;
    
    // ngram_max_length = std::stoi(doc.child("character").value());
    // word = doc.child("fts").child("parser").child("stop_words").child("word");
    // while (static_cast<bool>(word)) {
    //     stop_words.emplace_back(word.child_value());
    //     word = word.next_sibling("word");
    // }

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
    if (!loadSprites(path))    return false;

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
}