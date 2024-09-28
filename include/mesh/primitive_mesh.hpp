#pragma once
#include "objectTransform.hpp"
#include "../object/sprite.hpp"

class Primitive_mesh
{
public:
    Primitive_mesh(Sprite *_object, const objectTransform &_transform);
    Primitive_mesh(Sprite *_object);
    ~Primitive_mesh();
    void Render() const;
    objectTransform *GetTransform() { return &transform; }

private:
    objectTransform transform;
    Sprite *object;
};
