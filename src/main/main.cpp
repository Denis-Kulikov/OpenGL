#include "../include/glfw.hpp"
#include "../include/try.hpp"
#include <sys/time.h> 
#include <omp.h>

int main(int argc, char** argv)
{
    bool IsEnd = false;
    GLFWwindow* window = nullptr;
    InitializeGLFW(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    while (!IsEnd) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        IsEnd = RenderSceneCB();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
