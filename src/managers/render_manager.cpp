#include <managers/render_manager.hpp> 


void RenderManager::Initialize(GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat zNear, GLfloat zFar)
{
    render.pipeline.camera.SetPerspectiveProj(FOV, Width, Height, zNear, zFar);
}

void RenderManager::Dispose() {}

void RenderManager::PushCamera(const Camera &_camera)
{
    render.pipeline.camera = _camera;
}

void RenderManager::UpdateCamera()
{
    RenderManager::render.pipeline.camera.UpdateTarget();
}