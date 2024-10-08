#include <entities/templates/playable/Ghost.hpp>

Ghost::Ghost()
    : Character(std::string("player/Ghost"))
{
}

Ghost::~Ghost()
{}

void Ghost::Initialize()
{
    std::string path("player/Ghost");
    name = "Ghost";
}

std::string Ghost::GetName() const {
    return name;
}

// Mesh *Ghost::GetMesh() const {
//     return nullptr;
// }
