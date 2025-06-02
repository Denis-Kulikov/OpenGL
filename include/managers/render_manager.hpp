#pragma once
#include <render/render.hpp>
#include <object/primitive/sprite.hpp>
#include <object/primitive/cube.hpp>
#include <object/primitive/sphere.hpp>

class RenderManager  
{
public:
    static void Initialize(GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat zNear, GLfloat zFar);
    static void Dispose();

    static void PushCamera(ComponentCamera* camera);
    static void UpdateCamera();

    static void initializePrimitive();

    inline static Render render;

    inline static struct {
        Sprite* sprite = nullptr;
        Cube* cube = nullptr;
        Sphere<10>* sphere = nullptr;
    } primitives;

    inline static Camera::struct_PersProj PersProj;
};
