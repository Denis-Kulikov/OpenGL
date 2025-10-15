#include <managers/render/render.hpp>
#include <object/mesh/primitive/cube.hpp>


Cube::Cube() {
    const float size = 1.0f;
    float h = size * 0.5f;

    Positions = {
        // Front face
        { -h, -h,  h }, {  h, -h,  h }, {  h,  h,  h }, { -h,  h,  h },
        // Back face
        {  h, -h, -h }, { -h, -h, -h }, { -h,  h, -h }, {  h,  h, -h },
        // Left face
        { -h, -h, -h }, { -h, -h,  h }, { -h,  h,  h }, { -h,  h, -h },
        // Right face
        {  h, -h,  h }, {  h, -h, -h }, {  h,  h, -h }, {  h,  h,  h },
        // Top face
        { -h,  h,  h }, {  h,  h,  h }, {  h,  h, -h }, { -h,  h, -h },
        // Bottom face
        { -h, -h, -h }, {  h, -h, -h }, {  h, -h,  h }, { -h, -h,  h },
    };

    // деление текстуры на крест
    float x0 = 0.0f;
    float x1 = 0.25f;
    float x2 = 0.5f;
    float x3 = 0.75f;
    float x4 = 1.0f;

    float y0 = 0.0f;
    float y1 = 1.0f / 3.0f;
    float y2 = 2.0f / 3.0f;
    float y3 = 1.0f;

    TexCoords = {
        // Front (+Z) — между 0.25 и 0.5 по X, и между 1/3 и 2/3 по Y
        { x1, y1 }, { x2, y1 }, { x2, y2 }, { x1, y2 },

        // Back (-Z) — между 0.75 и 1.0
        { x3, y1 }, { x4, y1 }, { x4, y2 }, { x3, y2 },

        // Left (-X) — между 0.0 и 0.25
        { x0, y1 }, { x1, y1 }, { x1, y2 }, { x0, y2 },

        // Right (+X) — между 0.5 и 0.75
        { x2, y1 }, { x3, y1 }, { x3, y2 }, { x2, y2 },

        // Top (+Y) — между 0.25 и 0.5 по X, и между 2/3 и 1 по Y
        { x1, y2 }, { x2, y2 }, { x2, y3 }, { x1, y3 },

        // Bottom (-Y) — между 0.25 и 0.5 по X, и между 0 и 1/3
        { x1, y0 }, { x2, y0 }, { x2, y1 }, { x1, y1 },
    };

    // Нормали (по 1 направлению на грань)
    Normals = {
        // Front
        { 0.0f,  0.0f,  1.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f,  0.0f,  1.0f },
        // Back
        { 0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f, -1.0f },
        // Left
        { -1.0f,  0.0f,  0.0f }, { -1.0f,  0.0f,  0.0f }, { -1.0f,  0.0f,  0.0f }, { -1.0f,  0.0f,  0.0f },
        // Right
        { 1.0f,  0.0f,  0.0f }, { 1.0f,  0.0f,  0.0f }, { 1.0f,  0.0f,  0.0f }, { 1.0f,  0.0f,  0.0f },
        // Top
        { 0.0f,  1.0f,  0.0f }, { 0.0f,  1.0f,  0.0f }, { 0.0f,  1.0f,  0.0f }, { 0.0f,  1.0f,  0.0f },
        // Bottom
        { 0.0f, -1.0f,  0.0f }, { 0.0f, -1.0f,  0.0f }, { 0.0f, -1.0f,  0.0f }, { 0.0f, -1.0f,  0.0f },
    };

    Indices = {
        0, 1, 2, 2, 3, 0,       // front
        4, 5, 6, 6, 7, 4,       // back
        8, 9,10,10,11, 8,       // left
        12,13,14,14,15,12,      // right
        16,17,18,18,19,16,      // top
        20,21,22,22,23,20       // bottom
    };

    m_Entries.resize(1);
    m_Entries[0].NumIndices = static_cast<unsigned int>(Indices.size());
    m_Entries[0].BaseVertex = 0;
    m_Entries[0].BaseIndex  = 0;
}
