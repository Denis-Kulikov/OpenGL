#pragma once
#include "../../lib-project/lib.hpp"

class UnitInfo
{
public:
    static UnitInfo* UnitFire(Vector3<float> position, float direction, int id) {
        UnitInfo* unitInfo = new UnitInfo; 

        unitInfo->position = position;
        unitInfo->deg = direction;
        unitInfo->rotate = DegToPnt(unitInfo->deg);
        // unitInfo->deg = -ToDegree(unitInfo->deg);
        unitInfo->id = id;

        std::cout << "Fire: " << unitInfo->position << "   " << unitInfo->rotate << "   " << unitInfo->id << std::endl;

        return unitInfo;
    };

    Vector3<float> position;
    Vector3<float> rotate;
    float deg;
    int id;
    static inline const float unit_radius = 1.0f;
    static inline const float bullet_radius = 0.5f;
};
