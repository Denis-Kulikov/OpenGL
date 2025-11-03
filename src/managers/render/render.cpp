#include <managers/render/render.hpp> 
#include <managers/window/window_manager.hpp> 
#include <managers/window/window.hpp> 
#include <object/material/shader.hpp>

void RenderManager::Initialize(GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat zNear, GLfloat zFar)
{
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error: Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    PersProj = {FOV, Width, Height, zNear, zFar};
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.12f, 0.12f, 0.12f, 0.0f);
    WindowManager::curWindow->SwapBuffer();

    initializePrimitive();
    Texture::Create("white", "assets/img/white.png");
    Shader::Create("ShadowMapPerspective", "shaders/shadow_map/perspective_fs.glsl", "shaders/shadow_map/perspective_vs.glsl");
    Shader::Create("ShadowMapOrtho", "shaders/shadow_map/ortho_fs.glsl", "shaders/shadow_map/ortho_vs.glsl");

    bufferManager.Init();
}

void RenderManager::Dispose() {}

void RenderManager::SetCamera(ComponentCamera* camera)
{
    pipeline.camera = camera;
    pipeline.UpdatePerspective();
}

void RenderManager::UpdateCamera()
{
    if (RenderManager::pipeline.camera != nullptr) {
        RenderManager::pipeline.camera->camera.UpdateTarget();
    }
}

void RenderManager::initializePrimitive()
{
    // primitives.sprite = new Sprite();
    // primitives.cube = new Cube();
    // primitives.sphere = new Sphere<16>();
}
