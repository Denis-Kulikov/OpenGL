
#pragma once
#include "window.hpp"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <scene/scene.hpp>

#include <ui/visitor/component/title.hpp>
#include <ui/visitor/component/controller.hpp>

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
    static void DrawSceneHierarchy(Scene* scene);
    static void DrawInspectorActor();
    static void DrawInspectorComponent();
    static void DrawResources();

    static void DrawComponentTree(Component* node);

    static inline Actor* selectedActor = nullptr;
    static inline Component* selectedComponent = nullptr;
    static inline UIVisitorComponentTitle visitorCompTitle;
    static inline UIVisitorComponentController visitorCompController;
};
