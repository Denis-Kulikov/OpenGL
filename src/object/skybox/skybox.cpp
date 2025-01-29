#include <object/skybox/skybox.hpp>


Skybox::Skybox(const std::vector<std::string>& faces)
    : faces(faces), skyboxVAO(0), skyboxVBO(0), cubemapTexture(0) {}

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteTextures(1, &cubemapTexture);
}

void Skybox::Load() {
    struct face {
        face(Vector3<GLfloat> x, Vector3<GLfloat> y, Vector3<GLfloat> z) {
            vertex[0] = x;
            vertex[1] = y;
            vertex[2] = z;
        }

        Vector3<GLfloat> vertex[3];
    };


    std::vector<Vector3<GLfloat>> skyboxVertices = {
        // стартующая вершина
        {1, 1, 1},
        // передняя грань   (начало: ближний сверху справа)
        {-1, 1, 1},     {-1, -1, 1},
        // {1, 1, 1},
        {1, -1, 1},     {-1, -1, 1},

        // нижняя грань     (начало: ближний снизу слева)
        {-1, -1, -1},   {1, -1, -1},
        // {-1, -1, 1},
        {1, -1, 1},     {1, -1, -1},

        // задняя грань     (начало: дальний снизу справа)
        {-1, -1, -1},   {-1, 1, -1},
        // {1, -1, -1},
        {1, 1, -1},     {-1, 1, -1},

        // левая грань     (начало: дальний сверху слева)
        {-1, -1, -1},   {-1, -1, 1},
        // {-1, 1, -1},
        // смещение точки в {-1, -1, 1} в индексном буфере
        {-1, 1, 1},     {-1, 1, -1},

        // верхняя грань     (начало: дальний сверху слева)
        {-1, 1, 1},     {1, 1, 1},
        // {-1, 1, -1},
        {1, 1, -1},     {1, 1, 1},

        // правая грань     (начало: ближний сверху справа)
        {1, 1, -1},     {1, -1, -1},
        // {1, 1, 1},
        {1, -1, 1},     {1, -1, -1}
    };

    std::vector<std::array<std::size_t, 3>> skyboxIndexces = {
        {0,  1,  2},  {0,  3,  4},
        {4,  5,  6},  {4,  7,  8},
        {8,  9,  10}, {8,  11, 12},
        {12, 13, 14}, {14, 15, 16},
        {16, 17, 18}, {16, 19, 20},
        {20, 21, 22}, {20, 23, 24}
    };


    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::Render(void *RenderData) const {}

void Skybox::Render() {
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}
