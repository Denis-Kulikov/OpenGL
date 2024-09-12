#pragma once
#include "../object/objectTransform.hpp"


namespace light
{
class GlobalIllumination
{
    Vector3<GLfloat> Color;
    Vector3<GLfloat> Direction;
    float AmbientIntensity;
    float DiffuseIntensity;
};



}
