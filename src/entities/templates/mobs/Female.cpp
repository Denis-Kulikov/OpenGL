#include <entities/templates/mobs/Female.hpp>
#include <object/component/template/shape.hpp>
#include <object/component/template/mesh.hpp>
#include <object/transform/transform.hpp>
#include <object/sphere.hpp>


Female::Female()
    : Character(std::string(""))
{
    // ComponentCollaider *collider = CreateComponent<ComponentCollaider>();
    /*
    glm::vec3 scale(1);
    btSphereShape *colliderShape = new btSphereShape(scale.x);
    RigidTransform *rigidShape = new RigidTransform(btManager, colliderShape, 0.5);

    Transform *shapeTransform = new Transform();
    rigidShape->SetScale(scale);
    
    // glm::quat rotationt = glm::angleAxis(glm::radians(30.0f), glm::vec3(1, 0, 0));
    // rigidShape->SetScale(scale);
    // rigidShape->SetRotation(rotationt);
    // rigidShape->SetScale(scale);

    ComponentPhysic *shape = CreateComponent<ComponentPhysic>(rigidShape);
    // shape->shape = new Sphere<20>("img/brick_wall_10_diff_1k.jpg");

    rootComponent = shape;


    Transform *cmeshTransform = new Transform();
    Transform *meshTransform = new Transform();

    // float scaleMesh = 5.0f / scale.x;
    float scaleMesh = 3.0f;
    glm::quat rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1, 0, 0));
    meshTransform->SetPosition(glm::vec3(0.0, -1.0, 0.0));
    meshTransform->SetRotation(rotation);
    meshTransform->SetScale(glm::vec3(scaleMesh));

    ComponentMesh *cmesh = CreateComponent<ComponentMesh>(meshTransform);
    cmesh->mesh = Female::mesh;

    rootComponent->AddChild(cmesh);
    */
}

Female::~Female()
{}

void Female::Initialize()
{
    Female::name = "Female";
    std::string path("assets/model/female/female.glb");
}

std::string Female::GetName() const {
    return Female::name;
}
