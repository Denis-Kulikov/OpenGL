#include <render/glfw.hpp>
#include <render/render.hpp>
#include <object/scene.hpp>
#include <object/sphere.hpp>
#include <object/cube_bone.hpp>
#include <lib-project/try.hpp>

bool IsEnd = false;

bool RenderSceneCB(Render *render, Scene *scene)
{
    std::vector<sprite*>::iterator it = scene->getIterator();

    while (it != scene->objects.end()) {
        render->drawObeject(**it);
        ++it;
    }

    return IsEnd;
}

static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    double speed_movement = 0.2;
    double speed_rotation = 0.1;

    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    Camera* camera = callbackData->camera;

    switch (key) {
        case GLFW_KEY_F:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            IsEnd = true;
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

Scene *createScene()
{
    auto *scene = new Scene(std::string("Main scene"));

    sprite *mySprite;
    objectTransform _trans;
    mySprite = new sprite(std::string("Grass"), _trans, "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", "img/grass.png");
    mySprite->trans.Move(1, -1, 2);
    mySprite->trans.SetRotate(90, 0, 0);
    mySprite->trans.SetScale(3, 3, 0);
    scene->pushObject(mySprite);

    mySprite = new sprite(std::string("Wilson"), _trans, "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", "img/Wilson.png");
    mySprite->trans.Move(1, -1 + (mySprite->trans.Scale.y), 2);
    scene->pushObject(mySprite);

    mySprite = new sprite(std::string("MySprite3"), _trans, "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", "img/ball.png");
    mySprite->trans.Move(-3, 1, 5);
    scene->pushObject(mySprite);

    mySprite = new sphere(std::string("MySphere"), _trans, "shaders/sphere_fs.glsl", "shaders/base_vs.glsl", nullptr, 4);
    mySprite->trans.Move(-3, -3, 7);
    scene->pushObject(mySprite);

    mySprite = new cube_bone(std::string("MyCube"), _trans, Vector3<GLfloat>(1, 1, 1));
    mySprite->trans.Move(0, -1, 3);
    scene->pushObject(mySprite);

    return scene;
}

Camera *createCamera(int width, int height)
{
    auto camera = new Camera();

    Vector3<GLfloat> CameraPos(0.0f, 0.1f, -3);
    Vector3<GLfloat> CameraTarget(0.0f, 0.0f, 1.0f);
    Vector3<GLfloat> CameraUp(0.0f, 1.0f, 0.0f);

    camera->SetCamera(CameraPos, CameraTarget, CameraUp);
    camera->SetPerspectiveProj(60.0f, width, height, 0.5f, 1000.0f);

    return camera;
}

Render *InitializeGLFW(GLFWwindow* &window, int width, int height)
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
    glfwSetKeyCallback(window, KeyboardCB);
    glfwSetWindowUserPointer(window, callbackData);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.12f, 0.12f, 0.12f, 0.0f);

    createScene();

    Render *render = new Render(camera);

    return render;
}
