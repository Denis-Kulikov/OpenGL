#include <game/gameManager.hpp> 

bool GameManager::IsEnd = false;

const char Bone::BONE[] = "bone";
const char Bone::NAME[] = "name";

void GameManager::InitializeObjects()
{
    sprite::initializeGeometry();
    line::initializeGeometry();
    cube_bone::initializeGeometry();
}

Camera *GameManager::createCamera(int width, int height)
{
    auto camera = new Camera();

    Vector3<GLfloat> CameraPos(0.0f, 0.1f, -3);
    Vector3<GLfloat> CameraTarget(0.0f, 0.0f, 1.0f);
    Vector3<GLfloat> CameraUp(0.0f, 1.0f, 0.0f);

    camera->SetCamera(CameraPos, CameraTarget, CameraUp);
    camera->SetPerspectiveProj(60.0f, width, height, 0.5f, 1000.0f);

    return camera;
}

void GameManager::KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    double speed_movement = 0.25;
    double speed_rotation = 0.125;

    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    Camera* camera = callbackData->camera;

    switch (key) {
        case GLFW_KEY_F:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            GameManager::IsEnd = true;
            break;
        case GLFW_KEY_W:
            camera->Params.WorldPos.z += speed_movement;
            break;
        case GLFW_KEY_S:
            camera->Params.WorldPos.z -= speed_movement;
            break;
        case GLFW_KEY_D:
            camera->Params.WorldPos.x += speed_movement;
            break;
        case GLFW_KEY_A:
            camera->Params.WorldPos.x -= speed_movement;
            break;
        case GLFW_KEY_SPACE:
            camera->Params.WorldPos.y += speed_movement;
            break;
        case GLFW_KEY_C:
            camera->Params.WorldPos.y -= speed_movement;
            break;
        case GLFW_KEY_E:
            camera->Params.Target.x += speed_rotation;
            break;
        case GLFW_KEY_Q:
            camera->Params.Target.x -= speed_rotation;
            break;
    }
}

Render *GameManager::InitializeGLFW(GLFWwindow* &window, int width, int height)
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Game", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    Camera *camera = createCamera(width, height);
    CallbackData *callbackData = new CallbackData;
    callbackData->camera = camera;

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, GameManager::KeyboardCB);
    glfwSetWindowUserPointer(window, callbackData);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.12f, 0.12f, 0.12f, 0.0f);

    Render *render = new Render(camera);

    return render;
}