#pragma once
#include "../object/objectTransform.hpp"
#include "../object/sprite.hpp"
#include "../object/transform/transform.hpp"

class Shape
{
public:
    Shape(Sprite *_object, const Transform &_transform);
    Shape(Sprite *_object);
    ~Shape();
    void Render() const;
    Transform *GetTransform() { return &transform; }

private:
    Transform transform;
    Sprite *object;
};
