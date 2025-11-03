#pragma once
#include <scene/scene.hpp>

class Editor 
{
public:
    Editor();

    void OpenWindow();
    void SaveEntity();
    void CloseWindow();

    Actor* enity;
};
