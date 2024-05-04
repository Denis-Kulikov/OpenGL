#include <render/glfw.hpp> 
#include <game/gameManager.hpp> 


int main(int argc, char** argv)
{
    struct sockaddr_in server_addr;
    int sockfd, n;
    char buffer[MAX_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);


    sprintf(buffer, "GetID");
    sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));

    n = recvfrom(sockfd, (char *)buffer, MAX_SIZE, 0, NULL, NULL);
    buffer[n] = '\0';
    printf("Получено от сервера: %s\n", buffer);

    int id = atoi(buffer);

    std::cout << "ID: " << id << std::endl;

    // if (id == MAX_PLAYERS) return -1;


    const int width = 1280, height = 640;
    GLFWwindow* window = nullptr;
    Render *render = GameManager::InitializeGLFW(window, width, height);
    GameManager::InitializeObjects();
    Scene *scene = createScene(id);

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
