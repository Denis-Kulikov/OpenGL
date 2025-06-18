#include <managers/render_manager.hpp> 

void RenderManager::Initialize(GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat zNear, GLfloat zFar)
{
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error: Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    PersProj = {FOV, Width, Height, zNear, zFar};
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.12f, 0.12f, 0.12f, 0.0f);
    initializePrimitive();

    Texture::Create("white", "img/white.png");
}

void RenderManager::Dispose() {}

void RenderManager::PushCamera(ComponentCamera* camera)
{
    pipeline.camera = camera;
}

void RenderManager::UpdateCamera()
{
    if (RenderManager::pipeline.camera != nullptr) {
        RenderManager::pipeline.camera->camera.UpdateTarget();
    }
}

void RenderManager::initializePrimitive()
{
    primitives.sprite = new Sprite();
    primitives.cube = new Cube();
    primitives.sphere = new Sphere<16>();
}
