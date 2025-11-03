#include <managers/global.hpp> 
#include <managers/render/render.hpp> 
#include <managers/time.hpp> 
#include <managers/window/window_manager.hpp> 
#include <managers/window/window.hpp> 
#include <GLFW/glfw3.h>

std::unordered_map<std::string, std::unique_ptr<Window>> WindowManager::windows;

void WindowManager::Initialize(int width, int height) {
    if (!glfwInit()) {
        std::cerr << "Error: Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    curWindow = CreateWindow(width, height, "Engine", nullptr, nullptr);

    curWindow->MakeContextCurrent();
    curWindow->SetCursorPosCallback(WindowManager::MouseCB);
    curWindow->SetKeyCallback(WindowManager::KeyboardCB);
    curWindow->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    curWindow->SetInputMode(GLFW_STICKY_KEYS, GL_TRUE);
    curWindow->SetPosition(0, 32);

    // CreateWindow(320, 200, "Engine2", nullptr, curWindow->GetWindow());
}

void WindowManager::Dispose()
{
    for (auto& m : windows) {
        m.second.get()->Dispose();
    }

    glfwTerminate();
}

void WindowManager::SetWindow(Window* window) {
    curWindow = window;
    BindFrameBuffer();
}

void WindowManager::BindFrameBuffer() {
    curWindow->BindFrameBuffer();
}

void WindowManager::SwapBuffer() {
    curWindow->SwapBuffer();
}

void WindowManager::MouseCB(GLFWwindow* window, double xpos, double ypos) {
    auto& buttons = curWindow->GetButtons();
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

    if (curWindow == nullptr || curWindow->GetScene() == nullptr || curWindow->GetScene()->curCamera == nullptr)
        return;
    
    Camera& camera = curWindow->GetScene()->curCamera->camera;
    
    camera.yaw += xOffset;
    camera.pitch -= yOffset;

    if (camera.pitch > 89.0f) camera.pitch = 89.0f;
    if (camera.pitch < -89.0f) camera.pitch = -89.0f;

    camera.SetYaw(camera.yaw);
    camera.SetPitch(camera.pitch);
}

void WindowManager::KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static bool keys[GLFW_KEY_LAST] = {false};
    auto& buttons = curWindow->GetButtons();

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
    
    if (curWindow == nullptr || curWindow->GetScene() == nullptr || curWindow->GetScene()->curCamera == nullptr) return;
    glm::vec3 front = -curWindow->GetScene()->curCamera->camera.Params.Target;
    front.y = 0.0f;
    front = glm::normalize(front);

    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, -1.0f, 0.0f)));

    glm::vec3 direction(0.0f);

    if (keys[GLFW_KEY_W]) direction += front;
    if (keys[GLFW_KEY_S]) direction -= front;
    if (keys[GLFW_KEY_D]) direction += right;
    if (keys[GLFW_KEY_A]) direction -= right;
    if (keys[GLFW_KEY_SPACE]) direction.y += 1.0f;
    if (keys[GLFW_KEY_LEFT_CONTROL]) direction.y -= 1.0f;

    if (glm::length(direction) > 0.0f) {
        direction = glm::normalize(direction);
    }

    player->SetDirection(direction);
}

Window* WindowManager::CreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
    auto window = std::make_unique<Window>(width, height, title, monitor, share);
    Window* ptr = window.get();
    windows[title] = std::move(window);
    return ptr;
}