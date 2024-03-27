#pragma once
#include "../../lib-project/lib.hpp"
#include "../../math/Vector3.hpp"


class Motion
{
public:
    Motion() {};

    // typedef std::function<float(float)> FunType;
    typedef std::function<void(float&, const float&)> FunTypeFloat;
    typedef std::function<void(Vector3<GLfloat>&, const Vector3<GLfloat>&)> FunTypeVector;

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


    Vector3<GLfloat> GetScaleRecursive(size_t &i) const
    {
        Vector3<GLfloat> res(1.0);
        const std::vector<rule> &Rule = *curRuleScale;
        if (Rule.empty()) return res;


        functions[Rule[i].fun](res.x, Rule[i++].arg);
        functions[Rule[i].fun](res.y, Rule[i++].arg);
        while (i < curRuleScale->size()) {
            if (Rule[i].fun == ADD || Rule[i].fun == TIME) {
                res += GetScaleRecursive(i);
            } else {
                functions[Rule[i].fun](res.x, Rule[i++].arg);
                functions[Rule[i].fun](res.y, Rule[i++].arg);
            }
        }

        return res;
    }

    Vector3<GLfloat> GetScale(const float timeStart, float duration) const
    {
        if (ruleScale.empty()) return Vector3<GLfloat>(1.0);
        
        timeSpan = std::fmod((currentTime - timeStart) / 1e9, duration);
        size_t j = 0;
        curRuleScale = &ruleScale[j].second;
        while (ruleScale[j].first <= timeSpan) {
            j++;
            if (j >= ruleScale.size()) return Vector3<GLfloat>(1.0);
            curRuleScale = &ruleScale[j].second;
        }

        size_t i = 0;
        return GetScaleRecursive(i);
    }


    Vector3<GLfloat> GetOffsetRecursive(size_t &i) const
    {
        Vector3<GLfloat> res(0.0);
        const std::vector<rule> &Rule = *curRuleFlip;
        if (Rule.empty()) return res;

        functions[Rule[i].fun](res.x, Rule[i++].arg);
        functions[Rule[i].fun](res.y, Rule[i++].arg);
        while (i < curRuleFlip->size()) {
            if (Rule[i].fun == ADD || Rule[i].fun == TIME) {
                res += GetOffsetRecursive(i);
            } else {
                functions[Rule[i].fun](res.x, Rule[i++].arg);
                functions[Rule[i].fun](res.y, Rule[i++].arg);
            }
        }

        return res;
    }

    Vector3<GLfloat> GetOffset(const float timeStart, float duration) const
    {
        if (ruleOffset.empty()) return 0.0;
        
        timeSpan = std::fmod((currentTime - timeStart) / 1e9, duration);
        size_t j = 0;
        curRuleOffset = &ruleOffset[j].second;
        while (ruleOffset[j].first <= timeSpan) {
            j++;
            if (j >= ruleOffset.size()) return 0.0;
            curRuleFlip = &ruleOffset[j].second;
        }

        size_t i = 0;
        return GetOffsetRecursive(i);
    }


    float GetFlipRecursive(size_t &i) const
    {
        float res = 0;
        const std::vector<rule> &Rule = *curRuleFlip;
        if (Rule.empty()) return res;

        functions[Rule[i].fun](res, Rule[i++].arg);
        while (i < curRuleFlip->size()) {
            if (Rule[i].fun == ADD || Rule[i].fun == TIME) {
                res += GetFlipRecursive(i);
            } else {
                functions[Rule[i].fun](res, Rule[i++].arg);
            }
        }

        return res;
    }

    float GetFlip(const float timeStart, float duration) const
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
    std::vector<std::pair<float, std::vector<rule>>> ruleFlip;
    std::vector<std::pair<float, std::vector<rule>>> ruleScale;
    std::vector<std::pair<float, std::vector<rule>>> ruleOffset;

    static inline float currentTime = 0.0;
    static inline float timeSpan = 0.0;
    static inline float flip = 0.0;
    static inline GLfloat anchorDirection = 0.0;
    static inline const std::vector<rule> *curRuleFlip = nullptr;
    static inline const std::vector<rule> *curRuleScale = nullptr;
    static inline const std::vector<rule> *curRuleOffset = nullptr;

    static inline std::vector<FunTypeFloat> functions = [](){
        std::vector<FunTypeFloat> initFunctions;
        initFunctions.push_back([](float &L_value, const float &R_value) { L_value += R_value; }); // ADD,
        initFunctions.push_back([](float &L_value, const float &R_value) { L_value *= R_value; }); // MULTIPLY,
        initFunctions.push_back([](float &L_value, const float &R_value) { L_value = sin(L_value) * R_value; }); // SIN,
        initFunctions.push_back([](float &L_value, const float &R_value) { L_value = cos(L_value) * R_value; }); // COS;

        initFunctions.push_back([](float &L_value, const float &R_value) { L_value = Motion::timeSpan * R_value; }); // TIME;
        return initFunctions;
    }(); 

    // static inline std::vector<FunTypeVector> offsets = [](){
    //     std::vector<FunTypeVector> initFunctions;
    //     initFunctions.push_back([](Vector3<GLfloat> &L_value, const Vector3<GLfloat> &R_value) { L_value += R_value; }); // ADD,
    //     initFunctions.push_back([](Vector3<GLfloat> &L_value, const Vector3<GLfloat> &R_value) { L_value *= R_value; }); // MULTIPLY,

    //     initFunctions.push_back([](Vector3<GLfloat> &L_value, const Vector3<GLfloat> &R_value) { L_value = Motion::timeSpan; L_value *= R_value; }); // TIME;
    //     initFunctions.push_back([](Vector3<GLfloat> &L_value, const Vector3<GLfloat> &R_value) { L_value = Vector3<GLfloat>(cos(ToRadian(Motion::anchorDirection)), sin(ToRadian(Motion::anchorDirection)), 0.0f); L_value *= R_value; }); // DIRECTION;
    //     return initFunctions;
    // }(); 

        // initFunctions.push_back([](float &L_value, const float &R_value) { L_value = Motion::_flip; }); // FLIP;
    // std::vector<std::pair<GLfloat, FunType*()>> motion;
};
