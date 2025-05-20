#include <managers/render_manager.hpp>
#include <mesh/shape.hpp>

Shape::Shape(Sprite *_object, const Transform &_transform)
    : Shape(_object)
{
    transform = _transform;
}


Shape::Shape(Sprite *_object)
    : object (_object)
{}

Shape::~Shape()
{
    delete object;
}

void Shape::Render() const
{
    auto mat4x4 = RenderManager::render.GetPV() * RenderManager::render.pipeline.GetModel(transform);
    object->Render(&mat4x4);
}
