#pragma once
#include <mutex> 
#include "../object/sprite.hpp"
#include "../data/bit_array.hpp"
#include "../data/bit_big_array.hpp"


class CustomMesh : public Sprite
{
public:
    CustomMesh(vec3i size, BitBigArray &data, int partIndex);
    ~CustomMesh();
    void initializeGeometry(vec3i size, BitBigArray &data, int partIndex);
    struct GeometryInfo *GetGeometry() override;

    void Render(void *RenderData) const override;

protected:

    enum Face {
        LEFT,
        RIGHT,
        BOTTOM,
        TOP,
        BACK,
        FRONT
    };
    
    GLuint gColorLocation = 0;
    glm::vec3 color = {1.0f, 1.0f, 1.0f};

    void addCube(std::vector<float>& vertices, std::vector<unsigned int>& indices, glm::vec3 pos);
    void addFace(std::vector<float>& vertices, std::vector<unsigned int>& indices,
                    glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
    struct GeometryInfo geometryInfo = {0, 0, 0, 0, 0};
};
