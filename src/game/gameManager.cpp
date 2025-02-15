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

void GameManager::MouseCB(GLFWwindow* window, double xpos, double ypos) {
    static float yaw = 90.0f;
    static float pitch = 0.0f;
    const float sensitivity = 10.0f;

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

    Character* player = callbackData.player;

    player->SetYaw(yaw);
    player->SetPitch(pitch);
}

void GameManager::UpdateCamera()
{
    const float yaw_speed = 120.0;
    const float pitch_speed = 100.0;
    const float pitch_limit = 90.0;

    Character& player = *callbackData.player;

    render.pipeline.camera.Params.Target.x = -cos(glm::radians(player.GetYaw()));
    render.pipeline.camera.Params.Target.z = -sin(glm::radians(player.GetYaw()));
    render.pipeline.camera.Params.Target.y = tan(glm::radians(player.GetPitch()));
    render.pipeline.camera.PersProj.FOV = pitch_limit - fabs(player.GetPitch());

    // std::cout << "pitch: " << player.GetPitch() << " | " << "yaw: " << player.GetYaw() << std::endl;
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

    player->SetDirection(glm::vec3(
        keys[GLFW_KEY_D] - keys[GLFW_KEY_A], 
        keys[GLFW_KEY_SPACE] - keys[GLFW_KEY_LEFT_CONTROL],
        keys[GLFW_KEY_S] - keys[GLFW_KEY_W]
    ));
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

    #define win false 
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

    GLFWwindow* offscreen_context = glfwCreateWindow(1, 1, "", NULL, NULL); // Контекст без окна
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
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Создание текстуры для рендеринга
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Присоединение текстуры к framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Создание depth буфера
    glGenRenderbuffers(1, &depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete" << std::endl;
        exit(EXIT_FAILURE);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, _width, _height);

    #endif

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.12f, 0.12f, 0.12f, 0.0f);

    render.pipeline.camera.SetPerspectiveProj(70.0f, width, height, 0.5f, 7000.0f);
}