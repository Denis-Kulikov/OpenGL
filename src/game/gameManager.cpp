#include <game/gameManager.hpp> 

bool GameManager::IsEnd = false;
int GameManager::width = 0;
int GameManager::height = 0;
CallbackData GameManager::callbackData{nullptr, nullptr};
Render *GameManager::render = nullptr;

void GameManager::InitializeObjects()
{
    Sprite::initializeGeometry();
    line::initializeGeometry();
    cube_bone::initializeGeometry();
}

void GameManager::PushCamera(Camera *_camera)
{
    callbackData.camera = _camera;
}

void GameManager::PushPlayerTransform(objectTransform *_transform)
{
    callbackData.transform = _transform;
}

Camera *GameManager::createCamera()
{
    auto camera = new Camera();

    Vector3<GLfloat> CameraPos(0.0f, 3.0f, 15);
    Vector3<GLfloat> CameraTarget(0.0f, -0.3f, -1.0f);
    Vector3<GLfloat> CameraUp(0.0f, 1.0f, 0.0f);

    camera->SetCamera(CameraPos, CameraTarget, CameraUp);
    camera->SetPerspectiveProj(60.0f, width, height, 0.5f, 1000.0f);

    return camera;
}

void GameManager::KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    double speed_movement = -0.25;
    double speed_rotation = -0.125;

    // CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    objectTransform* transform = callbackData.transform;
    Camera* camera = callbackData.camera;

    switch (key) {
        case GLFW_KEY_F:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            GameManager::IsEnd = true;
            break;
        case GLFW_KEY_W:
            transform->WorldPos.z += speed_movement;
            break;
        case GLFW_KEY_S:
            transform->WorldPos.z -= speed_movement;
            break;
        case GLFW_KEY_D:
            transform->WorldPos.x += speed_movement;
            break;
        case GLFW_KEY_A:
            transform->WorldPos.x -= speed_movement;
            break;
        case GLFW_KEY_SPACE:
            transform->WorldPos.y -= speed_movement;
            break;
        case GLFW_KEY_C:
            transform->WorldPos.y += speed_movement;
            break;
        case GLFW_KEY_E:
            camera->Params.Target.x += speed_rotation;
            break;
        case GLFW_KEY_Q:
            camera->Params.Target.x -= speed_rotation;
            break;
    }
}

Render *GameManager::InitializeGLFW(GLFWwindow* &window, int _width, int _height)
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    width = _width;
    height = _height;

    window = glfwCreateWindow(width, height, "Game", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Camera *camera = createCamera();
    // callbackData = new CallbackData;
    callbackData.camera = nullptr;

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, GameManager::KeyboardCB);
    glfwSetWindowUserPointer(window, &callbackData);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.12f, 0.12f, 0.12f, 0.0f);

    render = new Render(callbackData.camera);

    return render;
}