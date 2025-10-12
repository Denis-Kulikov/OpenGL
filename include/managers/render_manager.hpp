#pragma once
#include <render/pipeline.hpp>
#include <object/mesh/primitive/sprite.hpp>
#include <object/mesh/primitive/cube.hpp>
#include <object/mesh/primitive/sphere.hpp>
#include "render/buffer_manager.hpp"

class RenderManager  
{
public:
    static void Initialize(GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat zNear, GLfloat zFar);
    static void Dispose();

    static void SetCamera(ComponentCamera* camera);
    static void UpdateCamera();

    static void initializePrimitive();

    inline static Pipeline pipeline;

    // inline static struct {
    //     Sprite* sprite = nullptr;
    //     Cube* cube = nullptr;
    //     Sphere<16>* sphere = nullptr;
    // } primitives;

    inline static Camera::struct_PersProj PersProj;
    inline static BufferManager bufferManager;
};
