#pragma once

#include "bone.hpp"

class Motion
{
public:
    Motion(std::vector<std::pair<std::string, float>> *_float, std::vector<std::pair<std::string, int>> *_int)
    {
        if (_float != nullptr) {
            for (auto &it : *_float)
                uniform_float.insert(it);
        }

        if (_int != nullptr) {
            for (auto &it : *_int)
                uniform_int.insert(it);
        }
    };
    Motion(std::vector<std::pair<std::string, float>> *_float) : Motion(_float, nullptr) {};
    Motion(std::vector<std::pair<std::string, int>> *_int) : Motion(nullptr, _int) {};
    Motion() {};
    ~Motion() {};

    typedef std::function<void()> FunType;

    struct bone_attribute
    {
        float offset[2];
        float flip;
        float scale[2];
    };

    void FindBoneRecursive(const std::string &name, const Bone &parent, size_t &bone, size_t &number)
    {
        for (const auto &it : parent.children) {
            number++;
            if (it.name == name) bone = number;
            if (bone) return;
            FindBoneRecursive(name, it, bone, number);
        }
    }

    size_t FindBone(std::string name)
    {
        size_t bone = 0;
        size_t number = 0;
        FindBoneRecursive(name, *skelet, bone, number);
        return bone;
    }

    float *FindUniformFloat(const std::string &uniform)
    {
        auto it = uniform_float.find(uniform);
        if (it != uniform_float.end()) {
            return &it->second;
        } else {
            return nullptr;
        }
    };

    void UniformFloat(const std::string &uniform, float new_value)
    {
        float *value = FindUniformFloat(uniform);

        if (value == nullptr)
            return;

        *value = new_value;
    };

    
    int *FindUniformInt(std::string &uniform)
    {
        auto it = uniform_int.find(uniform);
        if (it != uniform_int.end()) {
            return &it->second;
        } else {
            return nullptr;
        }
    };

    void UniformInt(std::string &uniform, int new_value)
    {
        int *value = FindUniformInt(uniform);

        if (value == nullptr)
            return;

        *value = new_value;
    };

    void PushTransform(Motion::bone_attribute *transform)
    {
        transformations = transform;
    };

    void PushSkelet(Bone *_skelet)
    {
        skelet = _skelet;
    };

    void PushTime(float time)
    {
        *FindUniformFloat(std::string("time")) = time;
    };

    void PushDuration(float time)
    {
        *FindUniformFloat(std::string("duration")) = time;
    };

    void PushMotion(const std::string &name, std::pair<float, FunType> &_motion)
    {
        function[name] = _motion;
    };


    Motion::bone_attribute *transformations = nullptr;
    Bone *skelet = nullptr;
    std::unordered_map<std::string, std::pair<float, FunType>> function;
    std::unordered_map<std::string, float> uniform_float = { { "time", 0.0 }, { "duration", 0.0 }};
    std::unordered_map<std::string, int> uniform_int;
};
