#include <entities/templates/decor/wooden_box.hpp>
#include <object/component/template/shape.hpp>
#include <object/cube.hpp>
#include <object/sphere.hpp>

WoodenBox::WoodenBox(BulletManager *btManagere)
    : Pawn(std::string(""))
{
    auto *cube = new Cube("img/box.jpg");
    {
        glm::vec3 scale(1, 1, 1);
        btBoxShape *colliderShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
        RigidTransform *rigidShape = new RigidTransform(btManagere, colliderShape, 0.0);
        Transform localTransform;
        localTransform.SetScale(scale);

        ComponentShape *shape = CreateComponent<ComponentShape>(nullptr, &localTransform, rigidShape);
        shape->shape = cube;

        rootComponent = shape;
    }
    {
        glm::vec3 scale(5, 5, 5);
        btBoxShape *colliderShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
        Transform *transformShape = new Transform();
        Transform localTransform;
        localTransform.SetPosition(glm::vec3(20, 0, 0));
        localTransform.SetScale(scale);

        ComponentShape *shape = CreateComponent<ComponentShape>(rootComponent, &localTransform, transformShape);
        shape->shape = cube;

        rootComponent->AddChild(shape);
    }
}

WoodenBox::~WoodenBox()
{}

void WoodenBox::Initialize()
{
    WoodenBox::name = "WoodenBox";
}

std::string WoodenBox::GetName() const {
    return WoodenBox::name;
}

Mesh *WoodenBox::GetMesh() const {
    return WoodenBox::mesh;
}

