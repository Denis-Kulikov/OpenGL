#include <game/gameManager.hpp> 

GameManager::GameManager() {};

GameManager::~GameManager()
{
    delete render;
    delete threads;
    delete callbackData.camera;

    glfwDestroyWindow(window);
}

void GameManager::PushCamera(Camera *_camera)
{
    callbackData.camera = _camera;
}

void GameManager::PushPlayer(Character *_player)
{
    callbackData.player = _player;
}

Camera *GameManager::createCamera()
{
    auto camera = new Camera();
    if (camera == nullptr) {
        std::cerr << "Error: " << "Failed to allocate memory to the camera" << std::endl;
        exit(EXIT_FAILURE);
    }

    Vector3<GLfloat> CameraPos(0.0f, 0.0f, 0.0f);
    Vector3<GLfloat> CameraTarget(0.0f, 0.0f, 1.0f);
    Vector3<GLfloat> CameraUp(0.0f, 1.0f, 0.0f);

    camera->SetCamera(CameraPos, CameraTarget, CameraUp);
    camera->SetPerspectiveProj(90.0f, width, height, 0.5f, 1000.0f);

    return camera;
}

void GameManager::MouseCB(GLFWwindow* window, double xpos, double ypos) {
    static float yaw = 90.0f;
    static float pitch = 0.0f;
    const float sensitivity = 4.0f;

    if (buttons.firstMouse) {
        buttons.lastX = xpos;
        buttons.lastY = ypos;
        buttons.firstMouse = false;
    }

    float xOffset = xpos - buttons.lastX;
    float yOffset = buttons.lastY - ypos;

    buttons.lastX = xpos;
    buttons.lastY = ypos;

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

    player->SetDirection(Vector3<GLfloat>(
        keys[GLFW_KEY_D] - keys[GLFW_KEY_A], 
        keys[GLFW_KEY_SPACE] - keys[GLFW_KEY_LEFT_CONTROL],
        keys[GLFW_KEY_S] - keys[GLFW_KEY_W]
    ));
}


void GameManager::UpdateCamera()
{
    const float yaw_speed = 120.0;
    const float pitch_speed = 100.0;
    const float pitch_limit = 90.0;

    Character& player = *callbackData.player;
    Camera& camera = *callbackData.camera;

    // player.SetYaw(player.GetYaw() + yaw_speed * buttons.yaw * Time.GetDeltaTime());
    // player.SetPitch(player.GetPitch() + pitch_speed * buttons.pitch * Time.GetDeltaTime());
    // if (fabs(player.GetPitch()) > pitch_limit) player.SetPitch(std::copysign(pitch_limit - 1e-3, player.GetPitch()));

    camera.Params.Target.x = -cos(ToRadian(player.GetYaw()));
    camera.Params.Target.z = -sin(ToRadian(player.GetYaw()));
    camera.Params.Target.y = tan(ToRadian(player.GetPitch()));
    camera.PersProj.FOV = pitch_limit - fabs(player.GetPitch());
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    width = _width;
    height = _height;

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
    glfwSetWindowPos(window, 320, 75);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error: " << "Failed to initialize glew" << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.12f, 0.12f, 0.12f, 0.0f);

    PushCamera(createCamera());

    render = new Render(callbackData.camera);
    if (render == nullptr) {
        std::cerr << "Error: " << "Failed to allocate memory to the renderer" << std::endl;
        exit(EXIT_FAILURE);
    }

    threads = new SceneThread();
    if (threads == nullptr) {
        std::cerr << "Error: " << "Failed to allocate memory to the threads" << std::endl;
        exit(EXIT_FAILURE);
    }
}