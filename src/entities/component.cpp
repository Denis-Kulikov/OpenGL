#include <entities/component.hpp>

bool addChildComponent(const std::string &_name, const objectTransform &_trans, const objectTransform &_anchorPoint, const component *_parent, const int _zIndex)
{
    auto _component = new component(_name, _trans, _anchorPoint, this, _zIndex)
    if (_component == nullptr) {
        std::cout << "Error addChildComponent: new component" << std::endl;
        return false;
    }

    children.push_back(_component);
    return true;
}

component::component(const std::string &_name, const objectTransform &_trans, const objectTransform &_anchorPoint, const component *_parent, const int _zIndex)
    : name(_name), trans(_trans), anchorPoint(_anchorPoint), parent(_parent), zIndex(_zIndex)
{
    trans.SetScale   (trans.x * parent.trans.x, trans.y * parent..y, parent..z * trans.z);
    trans.SetWorldPos(trans.x + parent.trans.x, trans.y + parent..y, parent..z + trans.z);
    trans.SetRotat   (trans.x + parent.trans.x, trans.y + parent..y, parent..z + trans.z);
}
