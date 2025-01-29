#pragma once
#include "../object/objectTransform.hpp"
#include "../object/sprite.hpp"

class PhysicsShape
{
public:
    PhysicsShape(Sprite *_object, const objectTransform &_transform);
    PhysicsShape(Sprite *_object);
    ~PhysicsShape();
    void Render() const;
    //objectTransform *GetTransform() { return &transform; }

private:
    Sprite *object;
};
