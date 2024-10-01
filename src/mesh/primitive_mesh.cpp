#include <game/gameManager.hpp>
#include <mesh/primitive_mesh.hpp>

Primitive_mesh::Primitive_mesh(Sprite *_object, const objectTransform &_transform)
    : Primitive_mesh(_object)
{
    transform = _transform;
}


Primitive_mesh::Primitive_mesh(Sprite *_object)
    : object (_object)
{}

Primitive_mesh::~Primitive_mesh()
{
    delete object;
}

void Primitive_mesh::Render() const
{
    auto mat4x4 = GameManager::render.PV * GameManager::render.pipeline.GetWorld(transform);
    return object->Render(&mat4x4);
}
