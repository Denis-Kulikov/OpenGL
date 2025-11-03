#pragma once
#include <string>
#include <unordered_map>
#include <memory>

class Window;
struct GLFWwindow;
struct GLFWmonitor;

class WindowManager  
{
public:
    static void Initialize(int width, int height);
    static void Dispose();

    static void SetWindow(Window* window);
    static void BindFrameBuffer();
    static void SwapBuffer();
    static void MouseCB(GLFWwindow* window, double xpos, double ypos);
    static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods);

    static Window* CreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);

    static std::unordered_map<std::string, std::unique_ptr<Window>> windows;
    inline static Window* curWindow;
};
