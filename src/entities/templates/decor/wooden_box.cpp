#include <entities/templates/decor/wooden_box.hpp>
#include <object/sphere.hpp>


void WoodenBox::BuildSpiral(ComponentShape *parent, int step, int maxSteps) {
    if (step >= maxSteps) return;

    float angle = glm::radians(step * 30.0f); // Поворачиваем каждый виток
    float radius = 2.0f; // Радиус спирали
    float heightStep = 2.0f; // Высота между витками

    Transform *transform = new Transform();

    glm::vec3 position(radius * cos(angle), step * heightStep, radius * sin(angle));
    // glm::vec3 position(0, step * heightStep, 0);
    transform->SetPosition(position);

    // Наклоняем куб по направлению движения спирали
    glm::quat rotation = glm::angleAxis(-angle, glm::vec3(0, 1, 0));
    transform->SetRotation(rotation);

    ComponentShape *shape = CreateComponent<ComponentShape>(transform);
    shape->shape = cube;
    parent->AddChild(shape);

    BuildSpiral(shape, step + 1, maxSteps);
}

WoodenBox::WoodenBox()
    : Pawn(std::string(""))
    {
        float angle = glm::radians(10.f); // Поворачиваем каждый виток
        float radius = 2.0f; // Радиус спирали
        float heightStep = 0.0f; // Высота между витками

        // glm::vec3 leftPos(radius * cos(angle), heightStep, radius * sin(angle));
        glm::vec3 leftPos(0, heightStep, 0);

        Transform *transform = new Transform();
        transform->SetPosition(leftPos);
        // transform->SetScale(glm::vec3(2));

        ComponentShape *shape = CreateComponent<ComponentShape>(transform);
        shape->shape = cube;
        rootComponent = shape;

        BuildSpiral(shape, 1, 100);
    }
// {
//     auto *cube = new Cube("img/box.jpg");
//     {
//         glm::quat rotationX = glm::angleAxis(glm::radians(45.0f), glm::vec3(1, 0, 0)); // 45° вокруг X
//         glm::quat rotationZ = glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 0, 1)); // 30° вокруг Z
//         glm::vec3 scale(2);
        
//         // btBoxShape *colliderShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
//         Transform *transformShape = new Transform();
//         Transform localTransform;
        
//         // transformShape->SetRotation(rotationX);
//         transformShape->SetScale(scale);
//         // localTransform.SetPosition(glm::vec3(6.0, -10.0 + scale.y, -10.0));

//         ComponentShape *shape = CreateComponent<ComponentShape>(nullptr, transformShape);
//         shape->shape = cube;

//         rootComponent = shape;
//     }
//     {
//         glm::vec3 scale(1);
//         glm::quat rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(1, 0, 0)); // 45° вокруг X
//         btBoxShape *colliderShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
//         Transform *transformShape = new Transform();
//         Transform localTransform;
//         transformShape->SetPosition(glm::vec3(3, 0, 0));
//         transformShape->SetRotation(rotation);
//         transformShape->SetScale(scale);

//         ComponentShape *shape = CreateComponent<ComponentShape>(rootComponent, transformShape);
//         shape->shape = cube;

//         rootComponent->AddChild(shape);
//     }
// }

WoodenBox::~WoodenBox()
{}

void WoodenBox::Initialize()
{
    WoodenBox::name = "WoodenBox";
    cube = new Cube("img/white.png");
}

std::string WoodenBox::GetName() const {
    return WoodenBox::name;
}

Mesh *WoodenBox::GetMesh() const {
    return WoodenBox::mesh;
}

