#include <entities/templates/decor/wooden_box.hpp>
#include <object/sphere.hpp>

WoodenBox::WoodenBox()
    : Pawn(std::string(""))
    {
        Transform *transform = new Transform();
        ComponentShape *shape = CreateComponent<ComponentShape>(transform);
        shape->shape = cube;
        rootComponent = shape;
    }

WoodenBox::~WoodenBox() {}

void WoodenBox::Initialize()
{
    WoodenBox::name = "WoodenBox";
    cube = new Cube("img/white.png");
}

std::string WoodenBox::GetName() const {
    return WoodenBox::name;
}
