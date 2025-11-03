#include <managers/window/window.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
    : width(width), height(height)
{
    window = glfwCreateWindow(width, height, title, monitor, share);

    if (!window) {
        glfwTerminate();
        std::cerr << "Error: Failed to create GLFW window" << std::endl;
        exit(EXIT_FAILURE);
    }
}
Window::~Window() {
    Dispose();
}

void Window::Dispose() {
    if (window != nullptr) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
}

void Window::MakeContextCurrent() {
    glfwMakeContextCurrent(window);
}

void Window::BindFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glClearDepth(1.0f);
}

void Window::SwapBuffer() {
    glfwSwapBuffers(window);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Hint(int hint, int value) {
    glfwWindowHint(hint, value);
}

void Window::SetInputMode(int mode, int value) {
    glfwSetInputMode(window, mode, value);
}

void Window::SetCursorPosCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(window, callback);
}

void Window::SetKeyCallback(GLFWkeyfun callback) {
    glfwSetKeyCallback(window, callback);
}

GLFWwindow* Window::GetWindow() {
    return window;
}

Window::Buttons& Window::GetButtons() {
    return buttons;
}

void Window::SetPosition(int xpos, int ypos) {
    glfwSetWindowPos(window, xpos, ypos);
}
