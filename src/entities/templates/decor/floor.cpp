#include <entities/templates/decor/floor.hpp>
#include <object/component/template/shape.hpp>
#include <object/cube.hpp>
#include <object/sphere.hpp>

Floor::Floor()
    : Pawn(std::string("Floor"))
{
    /*
    auto *floor = new Sprite("img/floor.jpg");

    glm::quat rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
    glm::vec3 scale(5.0f, 5.0f, 0.01f);
    glm::vec3 position(0.0f, 0.0f, 0.0f);

    Transform localTransform;
    

    btBoxShape *colliderShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
    RigidTransform *rigidShape = new RigidTransform(btManager, colliderShape, 0);

    rigidShape->SetPosition(position);
    rigidShape->SetRotation(rotation);
    rigidShape->SetScale(scale);

    ComponentShape *shape = CreateComponent<ComponentShape>(rigidShape);
    shape->shape = floor;

    rootComponent = shape;
    rootComponent->isMoved = true;
    */

    // ============

    // auto *floor = new Sprite("img/floor.jpg");

    // glm::quat rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
    // glm::vec3 position(0.0f, 0.0f, 0.0f);
    // glm::vec3 scale(1.0f, 5.0f, 1.0f);

    // Transform localTransform;
    
    // localTransform.SetPosition(position);
    // localTransform.SetRotation(rotation);
    // localTransform.SetScale(scale);

    // btBoxShape *colliderShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
    // Transform *rigidShape = new Transform();

    // ComponentShape *shape = CreateComponent<ComponentShape>(nullptr, &localTransform, rigidShape);
    // shape->shape = floor;

    // rootComponent = shape;
}


Floor::~Floor()
{}

void Floor::Initialize()
{
    Floor::name = "Floor";
}

std::string Floor::GetName() const {
    return Floor::name;
}

Mesh *Floor::GetMesh() const {
    return Floor::mesh;
}

