#include <managers/global_state.hpp> 
#include <managers/render_manager.hpp> 
#include <managers/time_manager.hpp> 
#include <managers/window_manager.hpp> 
#include <GLFW/glfw3.h>

void WindowManager::Initialize(GLfloat Width, GLfloat Height) {
    width = Width;
    height = Height;

    if (!glfwInit()) {
        std::cerr << "Error: Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Game", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Error: Failed to create GLFW window" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error: Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetCursorPosCallback(window, WindowManager::MouseCB);
    glfwSetKeyCallback(window, WindowManager::KeyboardCB);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetWindowPos(window, 150, 75);
}

void WindowManager::Dispose()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowManager::SwapBuffer() {
    glfwSwapBuffers(window);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowManager::MouseCB(GLFWwindow* window, double xpos, double ypos) {
    static float yaw = 0.0f;
    static float pitch = 0.0f;
    const float sensitivity = 20.0f;

    if (buttons.firstMouse) {
        buttons.lastX = xpos;
        buttons.lastY = ypos;
        buttons.firstMouse = false;
    }

    float xOffset = xpos - buttons.lastX;
    float yOffset = buttons.lastY - ypos;

    buttons.lastX = xpos;
    buttons.lastY = ypos;

    if (!buttons.cursor_disable) return;

    xOffset *= sensitivity * TimeManager::GetDeltaTime();
    yOffset *= sensitivity * TimeManager::GetDeltaTime();

    yaw -= xOffset;
    pitch += yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    if (RenderManager::render.camera == nullptr) return;
    RenderManager::render.camera->camera.SetYaw(yaw);
    RenderManager::render.camera->camera.SetPitch(pitch);
}

void WindowManager::KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static bool keys[GLFW_KEY_LAST] = {false};

    Actor* player = GlobalState::GetPlayer();

    if (action == GLFW_PRESS) 
        keys[key] = true;
    else if (action == GLFW_RELEASE) 
        keys[key] = false;

    if (keys[GLFW_KEY_F]) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        GlobalState::TerminateApp();
    }

    if (keys[GLFW_KEY_R]) {
        GlobalState::fScreenshot = true;
    }  

    if ((keys[GLFW_KEY_LEFT_ALT] || keys[GLFW_KEY_RIGHT_ALT]) && buttons.cursor_disable) {
        buttons.cursor_disable = !buttons.cursor_disable;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else if (!buttons.cursor_disable) {
        buttons.cursor_disable = !buttons.cursor_disable;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    if (RenderManager::render.camera == nullptr) return;
    glm::vec3 front = RenderManager::render.camera->camera.Params.Target;
    front.y = 0.0f;
    front = glm::normalize(front);

    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

    glm::vec3 direction(0.0f);

    if (keys[GLFW_KEY_W]) direction += front;
    if (keys[GLFW_KEY_S]) direction -= front;
    if (keys[GLFW_KEY_D]) direction += right;
    if (keys[GLFW_KEY_A]) direction -= right;
    if (keys[GLFW_KEY_SPACE]) direction += glm::vec3(0.0f, 1.0f, 0.0f);
    if (keys[GLFW_KEY_LEFT_CONTROL]) direction -= glm::vec3(0.0f, 1.0f, 0.0f);

    if (glm::length(direction) > 0.0f) {
        direction = glm::normalize(direction);
    }

    player->SetDirection(direction);
}
