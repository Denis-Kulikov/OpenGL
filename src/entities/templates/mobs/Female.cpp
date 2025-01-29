#include <entities/templates/mobs/Female.hpp>
#include <object/component/template/shape.hpp>
#include <object/component/template/mesh.hpp>
#include <object/component/physics/collaider.hpp>
#include <object/transform/transform.hpp>
#include <object/sphere.hpp>


Female::Female(BulletManager *btManagere)
    : Character(std::string(""))
{
    // ComponentShape *shape = CreateComponent<ComponentShape>();
    // // ComponentCollaider *collider = CreateComponent<ComponentCollaider>();
    // shape->shape = new Sphere<20>("img/brick_wall_10_diff_1k.jpg");
    // glm::vec3 scale(1);
    // btSphereShape *colliderShape = new btSphereShape(scale.x);
    // RigidTransform *rigidShape = new RigidTransform(btManagere, colliderShape, 1.5);
    // scale *= 2;
    // rigidShape->SetScale(scale);
    // shape->SetBody(rigidShape);
    // rootComponent = shape;

    // ComponentMesh *cmesh = CreateComponent<ComponentMesh>();
    // Transform *cmeshTransform = new Transform();
    // cmesh->mesh = Female::mesh;
    // cmesh->SetBody(cmeshTransform);

    // auto *t = cmesh->GetLocalTransform();
    // float scaleMesh = 5.0f / scale.x;
    // t->SetPosition(glm::vec3(30.0, -10.0, -10.0));
    // t->SetRotation(glm::quat(glm::radians(glm::vec3(-90.0f, 0.0f, 0.0f))));
    // t->SetScale(glm::vec3(scaleMesh, scaleMesh, scaleMesh));

    // rootComponent->AddChild(cmesh);

    // rootComponent->AddChild(cmesh);
    
}

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

