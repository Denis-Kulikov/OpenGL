#include <game/gameManager.hpp> 

int main(int argc, char** argv)
{
    const int width = 1600, height = 960;
    GameManager::InitializeGLFW(width, height);
    GameManager::InitializeObjects();

    std::unique_ptr<Scene> scene(createScene());
    GameManager::threads->setScene(scene.get());
    GameManager::threads->start();

    glfwDestroyWindow(GameManager::window);
    glfwTerminate();

    return 0;
}
