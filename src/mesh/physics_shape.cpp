#include <managers/render_manager.hpp>
#include <mesh/physics_shape.hpp>

PhysicsShape::PhysicsShape(Sprite *_object, const objectTransform &_transform)
    : PhysicsShape(_object)
{
    //transform = _transform;
}


PhysicsShape::PhysicsShape(Sprite *_object)
    : object (_object)
{}

PhysicsShape::~PhysicsShape()
{
    delete object;
}

void PhysicsShape::Render() const
{
    //auto mat4x4 = GameManager::render.PV * GameManager::render.pipeline.GetWorld(transform);
    //object->Render(&mat4x4);
}
