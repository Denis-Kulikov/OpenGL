
#pragma once
#include "window.hpp"
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <scene/scene.hpp>

class ImGuiManager  
{
public:
    static void Initialize(const std::string& version_GLSL = "#version 430");
    static void Dispose();
    static ImGuiIO& GetIO();
    static void NewFrame();
    static void Render(Scene* scene);

private:
    static void Render();
    static void DrawMainMenu();
    static void DrawSceneHierarchy(Scene* scene);
    static void DrawInspector(Scene* scene);
    static void DrawResources();
    static void DrawPerformance();

    static inline Actor* selectedActor = nullptr;
};
