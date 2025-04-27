#include <game/gameManager.hpp> 

GameManager::GameManager() {};

GameManager::~GameManager()
{
    glfwDestroyWindow(window);
}

void GameManager::PushCamera(const Camera &_camera)
{
    render.pipeline.camera = _camera;
}

void GameManager::PushPlayer(Character *_player)
{
    callbackData.player = _player;
}

void GameManager::UpdateCamera()
{
    GameManager::render.pipeline.camera.UpdateTarget();
}

void GameManager::MouseCB(GLFWwindow* window, double xpos, double ypos) {
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

    xOffset *= sensitivity * Time.GetDeltaTime();
    yOffset *= sensitivity * Time.GetDeltaTime();

    yaw -= xOffset;
    pitch += yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    render.pipeline.camera.SetYaw(yaw);
    render.pipeline.camera.SetPitch(pitch);
}

void GameManager::KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static bool keys[GLFW_KEY_LAST] = {false};

    Character* player = callbackData.player;

    if (action == GLFW_PRESS) 
        keys[key] = true;
    else if (action == GLFW_RELEASE) 
        keys[key] = false;

    if (keys[GLFW_KEY_F]) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        GameManager::IsEnd = true;
    }

    if ((keys[GLFW_KEY_LEFT_ALT] || keys[GLFW_KEY_RIGHT_ALT]) && buttons.cursor_disable) {
        buttons.cursor_disable = !buttons.cursor_disable;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else if (!buttons.cursor_disable) {
        buttons.cursor_disable = !buttons.cursor_disable;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    glm::vec3 front = render.pipeline.camera.Params.Target;
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

    callbackData.player->SetDirection(direction);
}

void GameManager::InitializeObjects()
{
    Sprite::initializeGeometry();
}

void GameManager::InitializeGLFW(int _width, int _height)
{
    if (!glfwInit()) {
        std::cerr << "Error: " << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    width = _width;
    height = _height;

    #define win true 
    #if win

    window = glfwCreateWindow(width, height, "Game", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Error: " << "Failed to create GLFW window" << std::endl;
        exit(EXIT_FAILURE);
    }


    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, GameManager::MouseCB);
    glfwSetKeyCallback(window, GameManager::KeyboardCB);
    glfwSetWindowUserPointer(window, &callbackData);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetWindowPos(window, 150, 75);


    if (glewInit() != GLEW_OK) {
        std::cerr << "Error: " << "Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    #else
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    offscreen_context = glfwCreateWindow(1, 1, "", NULL, NULL); // Контекст без окна
    if (!offscreen_context) {
        std::cerr << "Error: Failed to create offscreen OpenGL context" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(offscreen_context); // Устанавливаем контекст как текущий

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error: Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glGenFramebuffers(1, &framebuffer);
    glGenTextures(1, &texture);
    glGenRenderbuffers(1, &depthbuffer);

    #endif

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.12f, 0.12f, 0.12f, 0.0f);

    render.pipeline.camera.SetPerspectiveProj(70.0f, width, height, 0.5f, 7000.0f);
}