#pragma once

struct GLFWwindow;

class WindowManager  
{
public:
    static void Initialize(GLfloat Width, GLfloat Height);
    static void Dispose();

    static void SwapBuffer();
    static void MouseCB(GLFWwindow* window, double xpos, double ypos);
    static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    struct Buttons {
        int yaw = 0;
        int pitch = 0;

        float lastX = 400, lastY = 300;

        bool cursor_disable = true;
        bool firstMouse = true;
    };

    inline static Buttons buttons = {0, 0, 400, 300, true, true};
    inline static GLFWwindow* window = nullptr;
    inline static int width = 640;
    inline static int height = 480;
};
