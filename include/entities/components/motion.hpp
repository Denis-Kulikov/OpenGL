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

    float GetFlipRecursive(size_t &i) {
        float res = 0;

        functions[ruleFlip[i].fun](res, ruleFlip[i].arg);
        i++;
        while (i < ruleFlip.size()) {
            if (ruleFlip[i].fun == ADD) {
                res += GetFlipRecursive(i);
            } else {
                functions[ruleFlip[i].fun](res, ruleFlip[i].arg);
                i++;
            }
        }
        return res;
    }

    float GetFlip() {
        if (ruleFlip.empty()) return 0.0;
        
        size_t i = 0;
        return GetFlipRecursive(i);
    }

    static void PushTime(const float __time)
    {
        _time = __time;
    }


// protected:

    static inline float _time = 0;
    static inline float _flip = 0;
    static inline const Vector3<float> *_direction = nullptr;
    struct std::vector<rule> ruleFlip;

    static inline std::vector<FunType> functions = [](){
        std::vector<FunType> initFunctions;
        initFunctions.push_back([](float &L_value, const float &R_value) { L_value += R_value; }); // ADD,
        initFunctions.push_back([](float &L_value, const float &R_value) { L_value *= R_value; }); // MULTIPLY,
        initFunctions.push_back([](float &L_value, const float &R_value) { L_value += sin(R_value); }); // SIN,
        initFunctions.push_back([](float &L_value, const float &R_value) { L_value += cos(R_value); }); // COS;

        initFunctions.push_back([](float &L_value, const float &R_value) { L_value = Motion::_time; }); // TIME;
        // initFunctions.push_back([](float &L_value, const float &R_value) { L_value = Motion::_flip; }); // FLIP;
        // initFunctions.push_back([](float &L_value, const float &R_value) { L_value = Motion::_direction; }); // DIRECTION;
        return initFunctions;
    }(); 

    // std::vector<std::pair<GLfloat, FunType*()>> motion;
};
