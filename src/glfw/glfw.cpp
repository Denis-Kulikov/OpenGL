#include <render.hpp>
#include <scene.hpp>
#include <sphere.hpp>
#include <cube_bone.hpp>
#include <try.hpp>

int width = 1600;
int height = 960;

bool IsEnd = false;

// Scene mainScene(std::string("Main scene"));

Render *render;
sprite *mySprite[5];

bool RenderSceneCB()
{
    render->drawObeject(*mySprite[0]);
    render->drawObeject(*mySprite[1]);
    render->drawObeject(*mySprite[2]);
    render->drawObeject(*mySprite[3]);
    render->drawObeject(*mySprite[4]);
    render->drawObeject(*mySprite[5]);

    return IsEnd;
}

static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    double speed_movement = 0.2;
    double speed_rotation = 0.1;
    switch (key) {
        case GLFW_KEY_F:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            IsEnd = true;
            break;
        case GLFW_KEY_W:
            render->pipeline.camera->Params.WorldPos.z += speed_movement;
            break;
        case GLFW_KEY_S:
            render->pipeline.camera->Params.WorldPos.z -= speed_movement;
            break;
        case GLFW_KEY_D:
            render->pipeline.camera->Params.WorldPos.x += speed_movement;
            break;
        case GLFW_KEY_A:
            render->pipeline.camera->Params.WorldPos.x -= speed_movement;
            break;
        case GLFW_KEY_SPACE:
            render->pipeline.camera->Params.WorldPos.y += speed_movement;
            break;
        case GLFW_KEY_C:
            render->pipeline.camera->Params.WorldPos.y -= speed_movement;
            break;
        case GLFW_KEY_E:
            render->pipeline.camera->Params.Target.x += speed_rotation;
            break;
        case GLFW_KEY_Q:
            render->pipeline.camera->Params.Target.x -= speed_rotation;
            break;
    }
}

void InitializeGLFW(GLFWwindow* &window)
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    Vector3<GLfloat> CameraPos(0.0f, 0.1f, -3);
    Vector3<GLfloat> CameraTarget(0.0f, 0.0f, 1.0f);
    Vector3<GLfloat> CameraUp(0.0f, 1.0f, 0.0f);

    Pipeline _pipeline;
    _pipeline.camera = new Camera();
    _pipeline.object = nullptr;

    _pipeline.camera->SetCamera(CameraPos, CameraTarget, CameraUp);
    _pipeline.camera->SetPerspectiveProj(60.0f, width, height, 0.5f, 1000.0f);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Game", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, KeyboardCB);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.12f, 0.12f, 0.12f, 0.0f);

    {
        render = new Render(_pipeline);
        objectTransform _trans;
        mySprite[0] = new sprite(std::string("MySprite1"), _trans, "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", "img/floor.jpg");
        mySprite[0]->trans.Move(2, -1, 4);
        mySprite[1] = new sprite(std::string("MySprite2"), _trans, "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", "img/chess.jpg");
        mySprite[1]->trans.Move(-1, 2, 3);
        mySprite[2] = new sprite(std::string("MySprite3"), _trans, "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", "img/ball.png");
        mySprite[2]->trans.Move(-3, 1, 5);
        mySprite[3] = new sphere(std::string("MySphere"), _trans, "shaders/sphere_fs.glsl", "shaders/base_vs.glsl", nullptr, 8);
        mySprite[3]->trans.Move(-3, -3, 7);
        mySprite[4] = new cube_bone(std::string("MyCube"), _trans, Vector3<GLfloat>(1, 1, 1));
        mySprite[4]->trans.Move(0, -1, 3);
    }
}
