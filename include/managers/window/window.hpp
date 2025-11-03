#pragma once
#include <scene/scene.hpp>

struct GLFWwindow;
struct GLFWmonitor;
typedef void (*GLFWcursorposfun)(GLFWwindow*, double, double);
typedef void (*GLFWkeyfun)(GLFWwindow*, int, int, int, int);

class Window
{
public:
    struct Buttons {
        int yaw = 0;
        int pitch = 0;

        float lastX = 400, lastY = 300;

        bool cursor_disable = true;
        bool firstMouse = true;
    };

    Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
    ~Window();

    void Dispose();
    void MakeContextCurrent();
    void BindFrameBuffer();
    void SwapBuffer();

    void SetInputMode(int mode, int value);
    void SetCursorPosCallback(GLFWcursorposfun callback);
    void SetKeyCallback(GLFWkeyfun callback);
    void SetPosition(int xpos, int ypos);
    GLFWwindow* GetWindow();
    Scene* GetScene();
    float GetWidth();
    float GetHeight();
    Buttons& GetButtons();

    Scene* scene = nullptr;
    
protected:
    GLFWmonitor* monitor = nullptr;
    GLFWwindow* window = nullptr;
    int width;
    int height;
    Buttons buttons;
};