#include <entities/components/component.hpp>

bool Component::addChildComponent(const std::string &_name, const objectTransform &_trans, const objectTransform &_anchorPoint, const Component *_parent, const int _zIndex, const size_t _frameCount)
{
    auto _component = new Component(_name, _trans, _anchorPoint, this, _zIndex, _frameCount);
    if (_component == nullptr) {
        std::cout << "Error addChildComponent: new component" << std::endl;
        return false;
    }

    children.push_back(_component);
    return true;
}

Component::Component(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> &_anchorPoint, const Component *_parent, const int _zIndex, const size_t _frameCount)
    : name(_name), trans(_trans), anchorPoint(_anchorPoint), parent(_parent), zIndex(_zIndex), frameCount(_frameCount)
{
    trans.SetWorldPos(trans.WorldPos.x + parent.trans.WorldPos.x, trans.WorldPos.y + parent->trans.WorldPos.y, trans.WorldPos.z + parent->trans.WorldPos.z);
    trans.SetRotat   (trans.Rotate.x + parent.trans.Rotate.x, trans.Rotate.y + parent->trans.Rotate.y, trans.Rotate.z + parent->trans.Rotate.z);
    trans.SetScale   (trans.Scale.x * parent.trans.Scale.x, trans.Scale.y * parent->trans.Scale.y, trans.Scale.z * parent->trans.Scale.z);
}
