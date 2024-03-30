#include <render/glfw.hpp> 
#include <game/gameManager.hpp> 

int main(int argc, char** argv)
{
    const int width = 1600, height = 960;
    GLFWwindow* window = nullptr;
    Render *render = GameManager::InitializeGLFW(window, width, height);
    GameManager::InitializeObjects();
    Scene *scene = createScene();

    if (render == nullptr) {
        std::cout << "Error: render init" << std::endl;
        return -1;
    }

    while (!GameManager::IsEnd) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GameManager::IsEnd = RenderSceneCB(render, scene);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
