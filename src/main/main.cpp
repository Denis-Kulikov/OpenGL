#include <render/glfw.hpp> 
#include <game/gameManager.hpp> 

int main(int argc, char** argv)
{
    const int width = 1280, height = 640;
    GLFWwindow* window = nullptr;
    Render *render = GameManager::InitializeGLFW(window, width, height);
    GameManager::InitializeObjects();
    Scene *scene = createScene();

    if (render == nullptr) {
        std::cout << "Error: render init" << std::endl;
        return -1;
    }

    while (!GameManager::IsEnd) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        xpos -= width / 2;
        ypos -= height / 2;
        ypos = -ypos ;

        GameManager::deg = atan2(ypos, xpos);
        GameManager::deg = -ToDegree(GameManager::deg);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GameManager::IsEnd = RenderSceneCB(render, scene);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
