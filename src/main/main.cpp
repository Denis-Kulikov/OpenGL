#include <render/glfw.hpp> 
#include <game/gameManager.hpp> 

int main(int argc, char** argv)
{
    const int width = 1600, height = 960;
    Render *render = GameManager::InitializeGLFW(width, height);
    GameManager::InitializeObjects();

    if (render == nullptr) {
        std::cout << "Error: render init" << std::endl;
        return -1;
    }

    Scene *scene = createScene();
    GameManager::threads.setScene(scene);
    GameManager::threads.start();

    glfwTerminate();

    return 0;
}
