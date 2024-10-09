#pragma once
#include "sprite.hpp"
#include "../data/bit_big_array.hpp"
#include "../data/big_array.hpp"

class CubeSimple : public RenderableObject
{
public:
    CubeSimple();
    static void initializeGeometry();
    struct GeometryInfo *GetGeometry();
    void Render(void *RenderData) const override;
    void Drow(ull_I size, Matrix4f &Matrix, BitBigArray &data, int arrayIndex);
    void Drow(ull_I size, Matrix4f &Matrix, BigArray<bool> &data, int arrayIndex, int parts);

    struct CubeSimple_rdata {
        Matrix4f Matrix;
        GLuint instanceVBO;
        std::size_t size;
    };


private:
    inline static const std::size_t NUM_TREADS = 8; 

    Shader shader;
    GLuint gCommonMatrix;
    GLuint gColorLocation = 0;
    glm::vec3 color = {1.0f, 1.0f, 1.0f};
    static struct GeometryInfo geometryInfo;
};
