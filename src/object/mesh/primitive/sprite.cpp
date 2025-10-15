#include <object/mesh/primitive/sprite.hpp>

Sprite::Sprite()
{
    const float SIZE = 1.0f;
    float h = SIZE * 0.5f;

    Positions = {
        { -h, -h, 0.0f }, // левый нижний
        {  h, -h, 0.0f }, // правый нижний
        {  h,  h, 0.0f }, // правый верхний
        { -h,  h, 0.0f }  // левый верхний
    };

    TexCoords = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f }
    };

    Normals = {
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f }
    };

    Indices = {
        0, 1, 2,
        2, 3, 0
    };

    m_Entries.resize(1);
    m_Entries[0].NumIndices = static_cast<unsigned int>(Indices.size());
    m_Entries[0].BaseVertex = 0;
    m_Entries[0].BaseIndex  = 0;
}
