#pragma once
#include "sprite.hpp"
#include "../data/bit_array.hpp"

class CubeSimple : public RenderableObject
{
public:
    CubeSimple();
    static void initializeGeometry();
    struct GeometryInfo *GetGeometry();
    void Render(void *RenderData) const override;

    struct CubeSimple_rdata {
        std::size_t size;
        Matrix4f Matrix;
        BitArray *cubes;
    };


private:
    Shader shader;
    GLuint gCommonMatrix;
    GLuint gColorLocation = 0;
    glm::vec3 color = {1.0f, 1.0f, 1.0f};
    static struct GeometryInfo geometryInfo;
};
