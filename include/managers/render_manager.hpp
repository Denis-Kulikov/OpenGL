#pragma once
#include <render/pipeline.hpp>
#include <object/geometry/primitive/sprite.hpp>
#include <object/geometry/primitive/cube.hpp>
#include <object/geometry/primitive/sphere.hpp>

class RenderManager  
{
public:
    static void Initialize(GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat zNear, GLfloat zFar);
    static void Dispose();

    static void PushCamera(ComponentCamera* camera);
    static void UpdateCamera();

    static void initializePrimitive();

    inline static Pipeline pipeline;

    inline static struct {
        Sprite* sprite = nullptr;
        Cube* cube = nullptr;
        Sphere<16>* sphere = nullptr;
    } primitives;

    inline static Camera::struct_PersProj PersProj;
};
