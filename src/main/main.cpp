#include <render/glfw.hpp> 

int main(int argc, char** argv)
{
    int width = 1600, height = 960;
    GLFWwindow* window = nullptr;
    Render *render = InitializeGLFW(window, width, height);
    Scene *scene = createScene();
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    if (render == nullptr) {
        std::cout << "Error: render init" << std::endl;
        return -1;
    }

    bool IsEnd = false;
    while (!IsEnd) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        IsEnd = RenderSceneCB(render, scene);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
