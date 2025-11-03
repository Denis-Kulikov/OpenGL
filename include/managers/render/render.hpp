#pragma once
#include <object/mesh/primitive/sprite.hpp>
#include <object/mesh/primitive/cube.hpp>
#include <object/mesh/primitive/sphere.hpp>
#include "buffer.hpp"

class RenderManager  
{
public:
    static void Initialize();
    static void Dispose();

    inline static struct {
        Sprite sprite;
        Cube cube;
        // Sphere<16> sphere;
    } primitives;

    inline static BufferManager bufferManager;
};
