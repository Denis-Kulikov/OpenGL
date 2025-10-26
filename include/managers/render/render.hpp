#pragma once
#include <object/mesh/primitive/sprite.hpp>
#include <object/mesh/primitive/cube.hpp>
#include <object/mesh/primitive/sphere.hpp>
#include "buffer.hpp"
#include "pipeline.hpp"

class RenderManager  
{
public:
    static void Initialize(GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat zNear, GLfloat zFar);
    static void Dispose();

    static void SetCamera(ComponentCamera* camera);
    static void UpdateCamera();

    static void initializePrimitive();

    inline static Pipeline pipeline;

    inline static struct {
        Sprite sprite;
        Cube cube;
        // Sphere<16> sphere;
    } primitives;

    inline static Camera::struct_PersProj PersProj;
    inline static BufferManager bufferManager;
};
