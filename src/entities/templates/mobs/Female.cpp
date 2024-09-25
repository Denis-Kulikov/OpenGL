#include <entities/templates/mobs/Female.hpp>

Female::Female()
    : Character(std::string(""))
{}

Female::~Female()
{}

void Female::Initialize()
{
    Female::name = "Female";
    std::string path("assets/model/female/female.glb");
    Female::mesh = new Mesh(path);
}

std::string Female::GetName() const {
    return Female::name;
}

Mesh *Female::GetMesh() const {
    return Female::mesh;
}

