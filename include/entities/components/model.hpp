#pragma once
// #include "skelet.hpp"
// #include "components.hpp"
#include "bone.hpp"
// #include "../object/sprite.hpp"

class Model
{
public:
    Model() {};
    // Model(const std::string &_name, const objectTransform &_trans, const Vector3<GLfloat> &_anchorPoint, Component *_parent, const int _zIndex, const size_t _frameCount);

    // bool add_(const std::string &_name, const objectTransform &_trans, const objectTransform &_anchorPoint, Component *_parent, const int _zIndex, const size_t _frameCount);


    // bool linkModel();

    std::string name;
    objectTransform *trans;

    std::pair<Bone, std::string> skelets; // skelets=&overallSkelets

    // std::pair<Animations*, std::string> *animations; // список анимаций, который указывают на скелет и спрайты

protected:
    // static std::pair<Skelet, std::string>  overallSkelets; // по названию скелета
    // static std::pair<sprite, std::string> overallSprites; // общие 

    // std::pair<Components*, float> linkedModel; // float=bone.zIndex
    // std::pair<Bone, std::string> *skelets; // skelets=&overallSkelets
    // std::pair<sprite, std::string> *sprites; // sprites=&overallSprites;
};

    // vector<pair<int, double>> sortedVector(docs->begin(), docs->end());
    // sort(sortedVector.begin(), sortedVector.end(), [](const auto& lhs, const auto& rhs) { return lhs.second > rhs.second;});

