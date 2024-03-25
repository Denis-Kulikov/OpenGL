#pragma once
#include "../../lib-project/lib.hpp"
#include "../../math/Vector3.hpp"

class Motion
{
public:
    Motion() {};

    // typedef std::function<float(float)> FunType;
    typedef std::function<void(float&)> FunType;

    enum FUNTIONS {
        ADD,
        SIN,
        COS,

        MULTIPLY,
        TIME
        // FLIP,
        // DIRECTION
    };

    // const enum FUNTIONS fun;

    float GetFlipRecursive(size_t &i)
    {
        float res = 0;
        const std::vector<enum FUNTIONS> &RuleFlip = *curRuleFlip;
        if (RuleFlip.empty()) return res;

        functions[RuleFlip[i]](res);
        i++;
        while (i < curRuleFlip->size()) {
            if (RuleFlip[i] == ADD || RuleFlip[i] == TIME) {
                res += GetFlipRecursive(i);
            } else {
                functions[RuleFlip[i]](res);
                i++;
            }
        }

        return res;
    }

    float GetFlip(const float timeStart, float duration)
    {
        if (ruleFlip.empty()) return 0.0;
        
        timeSpan = std::fmod((currentTime - timeStart) / 1e9, duration);
        size_t j = 0;
        curRuleFlip = &ruleFlip[j].second;
        while (ruleFlip[j].first <= timeSpan) {
            j++;
            if (j >= ruleFlip.size()) return 0.0;
            curRuleFlip = &ruleFlip[j].second;
        }

        multiply = multipliers.empty() ? nullptr : multipliers.data();
        size_t i = 0;
        return GetFlipRecursive(i);
    }

    static void PushTime(const float _time)
    {
        currentTime = _time;
    }


// protected:
    std::vector<std::pair<float, std::vector<enum FUNTIONS>>> ruleFlip;
    std::vector<float> multipliers;

    static inline float currentTime = 0.0;
    static inline float timeSpan = 0.0;
    static inline float flip = 0.0;
    static inline const Vector3<float> *_direction = nullptr;
    static inline const std::vector<enum FUNTIONS> *curRuleFlip = nullptr;
    static inline const float *multiply = nullptr;

    static inline std::vector<FunType> functions = [](){
        std::vector<FunType> initFunctions;
        initFunctions.push_back([](float &L_value) { L_value = 1.0; }); // ADD,
        initFunctions.push_back([](float &L_value) { L_value = sin(L_value); }); // SIN,
        initFunctions.push_back([](float &L_value) { L_value = cos(L_value); }); // COS;

        initFunctions.push_back([](float &L_value) { if (Motion::multiply != nullptr) L_value *= *(Motion::multiply++); }); // MULTIPLY,
        initFunctions.push_back([](float &L_value) { L_value = Motion::timeSpan; }); // TIME;
        // initFunctions.push_back([](float &L_value, const float &R_value) { L_value = Motion::_flip; }); // FLIP;
        // initFunctions.push_back([](float &L_value, const float &R_value) { L_value = Motion::_direction; }); // DIRECTION;
        return initFunctions;
    }(); 

    // std::vector<std::pair<GLfloat, FunType*()>> motion;
};
