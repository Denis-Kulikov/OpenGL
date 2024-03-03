#pragma once
#include "bone.hpp"
#include "component.hpp"

class Skelet
{
public:
    Skelet(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> &_anchorPoint, Component *_parent, const int _zIndex, const size_t _frameCount);

    // bool addChildComponent(const std::string &_name, const objectTransform &_anchorPoint, const objectTransform &_trans, Component *_parent, const int _zIndex, const size_t _frameCount);
    bool addBone(const std::string &_name, const objectTransform &_anchorPoint, const objectTransform &_trans, Component *_parent, const int _zIndex, const size_t _frameCount);
    bool createList(); // создаёт список спрайтов, в порядке отрисовки

    addChildBone(std::stiring _parent, )
    {
        treeBones
    };


    std::pair<Bone*, float> createSkelet(cosnt std::string &path) 
    {
        // read file
        // 
    };

    std::string name;

    std::pair<Bone, std::string> treeBones; // дерево костей
    // std::pair<Components*, float> *listBones; // float = bone.zIndex

    // vector<pair<int, double>> sortedVector(docs->begin(), docs->end());
    // sort(sortedVector.begin(), sortedVector.end(), [](const auto& lhs, const auto& rhs) { return lhs.second > rhs.second;});

};
