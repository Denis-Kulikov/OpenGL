#include <render/glfw.hpp> 
#include <game/gameManager.hpp> 

void GameCallback()
{
    while (!GameManager::IsEnd) {
        double xpos, ypos;
        glfwGetCursorPos(GameManager::window, &xpos, &ypos);
        xpos -= GameManager::width / 2;
        ypos -= GameManager::height / 2;
        ypos = -ypos ;

        GameManager::deg = atan2(ypos, xpos);
        GameManager::deg = -ToDegree(GameManager::deg);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GameManager::IsEnd = RenderSceneCB(GameManager::render, GameManager::scene);
        glfwSwapBuffers(GameManager::window);
        glfwPollEvents();
    }
}


int main(int argc, char** argv)
{
    std::thread client_callback([&]() {
        std::cout << "client_callback" << std::endl;
        GameManager::client.connect_game();
        std::cout << "connected " << GameManager::client.GetID() << std::endl;
        bool wait = true;
        while (wait) {
            GameManager::client.mutex_lock();
            wait = GameManager::scene == nullptr;
            GameManager::client.mutex_unlock();
        }
        GameManager::client.scene = GameManager::scene;
        std::cout << "get scene" << std::endl;
        GameManager::client.callback();
    });
    client_callback.detach();

    bool wait = true;
    while(wait) {
        GameManager::client.mutex_lock();
        wait = GameManager::client.GetID() == 777;
        GameManager::client.mutex_unlock();
    }
    if (GameManager::client.GetID() >= MAX_PLAYERS) return -1;

    const int width = 1280, height = 640;
    Render *render = GameManager::InitializeGLFW(width, height);
    GameManager::InitializeObjects();
    Scene *scene = createScene(GameManager::client.GetID());
    GameManager::client.mutex_lock();
    GameManager::PushScene(scene);
    GameManager::client.mutex_unlock();

    GameCallback();

    glfwTerminate();
    GameManager::client.disconnect_game();

    return 0;
}
