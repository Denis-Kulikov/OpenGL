#pragma once
#include "../../lib-project/lib.hpp"
#include "../../math/Vector3.hpp"

class Motion
{
public:
    Motion() {};

    // typedef std::function<float(float)> FunType;
    typedef std::function<void(float&, const float&)> FunType;

    enum FUNTIONS {
        ADD,
        MULTIPLY,
        SIN,
        COS,
        TIME
        // FLIP,
        // DIRECTION
    };

    struct rule {
        const float arg;
        const enum FUNTIONS fun;
    };



    float GetFlipRecursive(size_t &i)
    {
        float res = 0;
        const std::vector<rule> &RuleFlip = *curRuleFlip;

        functions[RuleFlip[i].fun](res, RuleFlip[i].arg);
        i++;
        while (i < curRuleFlip->size()) {
            if (RuleFlip[i].fun == ADD || RuleFlip[i].fun == TIME) {
                res += GetFlipRecursive(i);
            } else {
                functions[RuleFlip[i].fun](res, RuleFlip[i].arg);
                i++;
            }
        }

        return res;
    }

    float GetFlip(const float timeStart)
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

        size_t i = 0;
        return GetFlipRecursive(i);
    }

    static void PushTime(const float _time)
    {
        currentTime = _time;
    }


// protected:
    float duration = 0.0;
    std::vector<std::pair<float, std::vector<rule>>> ruleFlip;

    static inline float currentTime = 0.0;
    static inline float timeSpan = 0.0;
    static inline float flip = 0.0;
    static inline const Vector3<float> *_direction = nullptr;
    static inline const std::vector<rule> *curRuleFlip = nullptr;

    static inline std::vector<FunType> functions = [](){
        std::vector<FunType> initFunctions;
        initFunctions.push_back([](float &L_value, const float &R_value) { L_value += R_value; }); // ADD,
        initFunctions.push_back([](float &L_value, const float &R_value) { L_value *= R_value; }); // MULTIPLY,
        initFunctions.push_back([](float &L_value, const float &R_value) { L_value += sin(L_value) * R_value; }); // SIN,
        initFunctions.push_back([](float &L_value, const float &R_value) { L_value += cos(L_value) * R_value; }); // COS;

        initFunctions.push_back([](float &L_value, const float &R_value) { L_value = Motion::timeSpan * R_value; }); // TIME;
        // initFunctions.push_back([](float &L_value, const float &R_value) { L_value = Motion::_flip; }); // FLIP;
        // initFunctions.push_back([](float &L_value, const float &R_value) { L_value = Motion::_direction; }); // DIRECTION;
        return initFunctions;
    }(); 

    // std::vector<std::pair<GLfloat, FunType*()>> motion;
};
