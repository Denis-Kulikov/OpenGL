#include <managers/render_manager.hpp> 

void RenderManager::Initialize(GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat zNear, GLfloat zFar)
{
    // render.pipeline.camera.SetPerspectiveProj(FOV, Width, Height, zNear, zFar);
    PersProj = {FOV, Width, Height, zNear, zFar};
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.12f, 0.12f, 0.12f, 0.0f);
    initializePrimitive();

    Texture::Create("white", "img/white.png");
}

void RenderManager::Dispose() {}

void RenderManager::PushCamera(ComponentCamera* camera)
{
    render.camera = camera;
}

void RenderManager::UpdateCamera()
{
    if (RenderManager::render.camera != nullptr) {
        RenderManager::render.camera->camera.UpdateTarget();
    }
}

void RenderManager::initializePrimitive()
{
    primitives.sprite = new Sprite();
    primitives.cube = new Cube();
}
