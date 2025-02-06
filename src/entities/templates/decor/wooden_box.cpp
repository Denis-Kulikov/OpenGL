#include <entities/templates/decor/wooden_box.hpp>
#include <object/component/template/shape.hpp>
#include <object/cube.hpp>
#include <object/sphere.hpp>

WoodenBox::WoodenBox(BulletManager *btManagere)
    : Pawn(std::string(""))
{
    auto *cube = new Cube("img/box.jpg");
    {
        glm::quat rotationX = glm::angleAxis(glm::radians(45.0f), glm::vec3(1, 0, 0)); // 45° вокруг X
        glm::quat rotationZ = glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 0, 1)); // 30° вокруг Z
        glm::vec3 scale(2);
        
        // btBoxShape *colliderShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
        Transform *transformShape = new Transform();
        Transform localTransform;
        
        // transformShape->SetRotation(rotationX);
        transformShape->SetScale(scale);
        // localTransform.SetPosition(glm::vec3(6.0, -10.0 + scale.y, -10.0));

        ComponentShape *shape = CreateComponent<ComponentShape>(nullptr, transformShape);
        shape->shape = cube;

        rootComponent = shape;
    }
    {
        glm::vec3 scale(1);
        glm::quat rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(1, 0, 0)); // 45° вокруг X
        btBoxShape *colliderShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
        Transform *transformShape = new Transform();
        Transform localTransform;
        transformShape->SetPosition(glm::vec3(3, 0, 0));
        transformShape->SetRotation(rotation);
        transformShape->SetScale(scale);

        ComponentShape *shape = CreateComponent<ComponentShape>(rootComponent, transformShape);
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

