#pragma once
#include "../object/objectTransform.hpp"
#include "../object/sprite.hpp"

class Shape
{
public:
    Shape(Sprite *_object, const objectTransform &_transform);
    Shape(Sprite *_object);
    ~Shape();
    void Render() const;
    objectTransform *GetTransform() { return &transform; }

private:
    objectTransform transform;
    Sprite *object;
};
