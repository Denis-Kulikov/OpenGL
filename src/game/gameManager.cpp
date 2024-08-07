#include <game/gameManager.hpp> 

GameManager::GameManager() {};

GameManager::~GameManager()
{
    delete render;
    delete threads;
    delete callbackData.camera;
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

    Vector3<GLfloat> CameraPos(0.0f, 3.0f, 15);
    Vector3<GLfloat> CameraTarget(0.0f, -0.3f, -1.0f);
    Vector3<GLfloat> CameraUp(0.0f, 1.0f, 0.0f);

    camera->SetCamera(CameraPos, CameraTarget, CameraUp);
    camera->SetPerspectiveProj(60.0f, width, height, 0.5f, 1000.0f);

    return camera;
}

void GameManager::KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static bool keys[GLFW_KEY_LAST] = {false};

    const GLfloat speed_rotation = -0.125;

    Character* player = callbackData.player;
    Camera* camera = callbackData.camera;

    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }

    if (keys[GLFW_KEY_F]) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        GameManager::IsEnd = true;
    }

    player->SetDirection(Vector3<GLfloat>(
        keys[GLFW_KEY_A] * 1.0 - keys[GLFW_KEY_D] * 1.0, 
        0, 
        keys[GLFW_KEY_S] * 1.0 - keys[GLFW_KEY_W] * 1.0
    ));

    if (keys[GLFW_KEY_SPACE]) {
    }

    if (keys[GLFW_KEY_C]) {
    }

    if (keys[GLFW_KEY_E]) {
        camera->Params.Target.x += speed_rotation;
    }

    if (keys[GLFW_KEY_Q]) {
        camera->Params.Target.x -= speed_rotation;
    }
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
    glfwSetKeyCallback(window, GameManager::KeyboardCB);
    glfwSetWindowUserPointer(window, &callbackData);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

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