#pragma once
#include "bone.hpp"
// #include "component.hpp"
// #include "skelet.hpp"
#include "../../object/sprite.hpp"

class Components
{
public:
    // Components(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> &_anchorPoint, Components *_parent, const int _zIndex, const size_t _frameCount);
    Components() {};

    // bool addChildComponent(const std::string &_name, const objectTransform &_anchorPoint, const objectTransform &_trans, Component *_parent, const int _zIndex, const size_t _frameCount);
    // bool addBone(const std::string &_name, const objectTransform &_anchorPoint, const objectTransform &_trans, Components *_parent, const int _zIndex, const size_t _frameCount);
    // std::pair<Bone*, float> createSkelet(cosnt std::string &path) 
    // {
    // };
    // bool linkModel();

    // std::string name;
    objectTransform trans;
    // objectTransform anchorPoint;
    Sprite *sprite;
    // float time = 0;

    // sprite *getSprite();             // получение спрайта от анимации
    // objectTransform *getTransform(); // получение данных trans от кости
};
