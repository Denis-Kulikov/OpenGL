#pragma once

#include "lib-project/lib.hpp"

class motion
{
public:
    motion(std::vector<std::pair<std::string, float>> *_float, std::vector<std::pair<std::string, int>> *_int)
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
    motion(std::vector<std::pair<std::string, float>> *_float) : motion(_float, nullptr) {};
    motion(std::vector<std::pair<std::string, int>> *_int) : motion(nullptr, _int) {};
    motion() {};
    ~motion() 
    {
        // delete function;
    };

    // void СalculateAnimation() =0;
    typedef std::function<void()> FunType;


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

    void PushTime(float time)
    {
        *FindUniformFloat(std::string("time")) = time;
    };

    void PushDuration(float time)
    {
        *FindUniformFloat(std::string("duration")) = time;
    };

    void PushTransform(float *transform)
    {
        transformations = transform;
    };

    void PushMotion(const std::string &name, std::pair<float, FunType> &_motion)
    {
        function[name] = _motion;
    };


    float *transformations = nullptr; // *bones[n * 5]

// private:
    std::unordered_map<std::string, std::pair<float, FunType>> function;
    std::unordered_map<std::string, float> uniform_float = { { "time", 0.0 }, { "duration", 0.0 }};
    std::unordered_map<std::string, int> uniform_int;
};


