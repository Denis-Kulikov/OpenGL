#include <glfw.hpp>
#include <try.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define DEBUG !true

GLuint VAO;
GLuint VBO;
GLuint EBO;
GLuint texCoordVBO;

GLuint gWorldLocation;
GLuint gScaleLocation;
GLuint gMassLocation;

GLuint ShaderSphere;
GLuint ShaderCube;
GLuint ShaderCircle;

GLuint TextureFloor;

int width = 1280;
int height = 768;

double radius = 0.275f;
int SPHERE_SEGMENTS = 4;
int numVertices;
int numIndices;

Pipeline pipeline;
Vector3f region;
bool IsEnd = false;

const char PATH_IMG_FLOOR[] = "img/floor.jpg";

void CreateCircle(size_t vert_count, const char texture[])
{
    #if DEBUG 
        std::cout << __LINE__ << std::endl; 
    #endif
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> texCoords;

    TRY(vert_count < 3, "Error: CreateList (vert < 3)");

    float deg = 360.0f / vert_count;
    for (size_t i = 0; i < vert_count; ++i) {
        float angle_rad = radians(deg * i);
        float x = cos(angle_rad);
        float y = sin(angle_rad);
        float z = 0.0f;

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);

        texCoords.push_back((x + 1) / 2);
        texCoords.push_back((y + 1) / 2);
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);


    glGenBuffers(1, &texCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniform1i(glGetUniformLocation(ShaderCircle, "textureSampler"), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void CreateCube()
{
    std::vector<GLfloat> vertices = {
        -1, -1, 1,
        -1, 1, 1,   -1, 1, -1,  -1, 1, 1, 
        1, 1, 1,    1, 1, -1,    1, 1, 1,
        1, -1, 1,   1, -1, -1,   1, -1, 1,
        -1, -1, 1,

        -1, -1, -1,
        -1, 1, -1,
        1, 1, -1,
        1, -1, -1,
        -1, -1, -1
    };

    numVertices = static_cast<int>(vertices.size()) / 3;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void createSphere()
{
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    for (int lat = 0; lat <= SPHERE_SEGMENTS; ++lat) {
        double theta = lat * M_PI / SPHERE_SEGMENTS;
        double sinTheta = sin(theta);
        double cosTheta = cos(theta);

        for (int lon = 0; lon <= SPHERE_SEGMENTS; ++lon) {
            double phi = lon * 2 * M_PI / SPHERE_SEGMENTS;
            double sinPhi = sin(phi);
            double cosPhi = cos(phi);

            float x = static_cast<float>(cosPhi * sinTheta);
            float y = static_cast<float>(cosTheta);
            float z = static_cast<float>(sinPhi * sinTheta);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    numVertices = static_cast<int>(vertices.size()) / 3;

    for (int lat = 0; lat < SPHERE_SEGMENTS; ++lat) {
        for (int lon = 0; lon < SPHERE_SEGMENTS; ++lon) {
            int current = lat * (SPHERE_SEGMENTS + 1) + lon;
            int next = current + SPHERE_SEGMENTS + 1;

            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }

    numIndices = static_cast<int>(indices.size());

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DrawCircle(size_t vert_count)
{
    #if DEBUG 
        std::cout << __LINE__ << std::endl; 
    #endif

    pipeline.object.SetWorldPos(0, 0, -region.z / 2);
    pipeline.object.SetRotate(15, 0, 0);
    pipeline.object.SetScale(radius, radius, 0);

    GLfloat floatMatrix[16];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) 
            floatMatrix[i * 4 + j] = static_cast<GLfloat>(pipeline.GetTrans()->m[i][j]);
    }

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, floatMatrix);

    GLuint textureSamplerLocation = glGetUniformLocation(ShaderCircle, "textureSampler");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureFloor);
    glUniform1i(textureSamplerLocation, 0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vert_count);
    glBindVertexArray(0);
}

void DrawCube()
{
    pipeline.object.SetWorldPos(0, 0, 0);
    pipeline.object.SetRotate(0, 0, 0);
    pipeline.object.SetScale(region.x, region.y, region.z);

    GLfloat floatMatrix[16];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) 
            floatMatrix[i * 4 + j] = static_cast<GLfloat>(pipeline.GetTrans()->m[i][j]);
    }

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, floatMatrix);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, numVertices);
    glBindVertexArray(0);
}

void DrawSphere()
{
    pipeline.object.SetWorldPos(0, 0, 0);
    pipeline.object.SetRotate(0, 0, 0);
    pipeline.object.SetScale(radius, radius, radius);

    GLfloat floatMatrix[16];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) 
            floatMatrix[i * 4 + j] = static_cast<GLfloat>(pipeline.GetTrans()->m[i][j]);
    }

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, floatMatrix);
    glUniform1f(gMassLocation, static_cast<float>(0.5));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

bool RenderSceneCB()
{
    #if DEBUG 
        std::cout << __LINE__ << std::endl;
    #endif

    glUseProgram(ShaderSphere); 
    createSphere();
    DrawSphere();

    glUseProgram(ShaderCube);
    CreateCube();
    DrawCube();

    size_t sizeCircle = 32;
    glUseProgram(ShaderCircle);
    CreateCircle(sizeCircle, PATH_IMG_FLOOR);
    DrawCircle(sizeCircle);

    return IsEnd;
}

GLuint LoadTexures(const char *texture_path)
{
    #if DEBUG 
        std::cout << __LINE__ << "\t" << texture_path << std::endl; 
    #endif

    GLuint textureID;
    int x, y, n;
    std::string path = std::string("assets/") + texture_path;
    unsigned char *img = stbi_load(path.c_str(), &x, &y, &n, 0);

    TRY(img == nullptr, string("Failed to load texture: " + path))

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, (n == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(img);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }

    return textureID;
}

GLuint LoadShader(const char *shader_path, GLuint type)
{
    #if DEBUG 
        std::cout << __LINE__ << "\t" << shader_path << std::endl; 
    #endif

    ifstream shader_file(shader_path);

    if (!shader_file.is_open()) {
        cerr << "Error: Could not open shader file '" << shader_path << "'" << endl;
        return 0;
    }

    stringstream shader_stream;
    shader_stream << shader_file.rdbuf();
    shader_file.close();

    char* code = (char*)malloc(shader_stream.str().length() + 1);
    for (size_t i = 0; i < shader_stream.str().length(); ++i) {
        code[i] = shader_stream.str()[i];
    }
    code[shader_stream.str().length()] = '\0';
    const GLchar* shader_code = code;

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);

    GLint ok;
    GLchar log[2000];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(shader, 2000, NULL, log);
        printf("Shader(%s): %s\n", shader_path, log);
        cout << shader_stream.str().c_str() << endl;
    }

    return shader;
}

void CompileShadersProgram(GLuint ShaderProgram, const char *FS, const char *VS)
{
    GLuint shader_color;
    GLuint shader_position;

    if (FS != nullptr) {
        shader_color = LoadShader(FS, GL_FRAGMENT_SHADER);
        glAttachShader(ShaderProgram, shader_color);
    }
    if (VS != nullptr) {
        shader_position = LoadShader(VS, GL_VERTEX_SHADER);
        glAttachShader(ShaderProgram, shader_position);
    }

    glLinkProgram(ShaderProgram);

    GLint ok;
    GLchar log[2000];
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(ShaderProgram, 2000, NULL, log);
        std::cout << "ShaderSphere Compilation Log:\n" << log << std::endl;
    
        if (FS != nullptr) {
            GLint infoLogLength;
            glGetShaderiv(shader_color, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar *infoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(shader_color, infoLogLength, NULL, infoLog);
            std::cout << "ShaderSphere shader_color Log:\n" << infoLog << std::endl;
            delete[] infoLog;
        }
        if (VS != nullptr) {
            GLint infoLogLength;
            glGetShaderiv(shader_position, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar *infoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(shader_position, infoLogLength, NULL, infoLog);
            std::cout << "ShaderSphere shader_position Log:\n" << infoLog << std::endl;
            delete[] infoLog;
        }
    }

    glUseProgram(ShaderProgram);
}

void CompileShaders()
{
    ShaderSphere = glCreateProgram();
    ShaderCube   = glCreateProgram();
    ShaderCircle = glCreateProgram();

    CompileShadersProgram(ShaderSphere, "shaders/sphere_fs.glsl", "shaders/sphere_vs.glsl");
    gWorldLocation = glGetUniformLocation(ShaderSphere, "gWorld");
    assert(gWorldLocation != 0xFFFFFFFF);

    CompileShadersProgram(ShaderCube, "shaders/cube_fs.glsl", "shaders/base_vs.glsl");
    gWorldLocation = glGetUniformLocation(ShaderCube, "gWorld");
    assert(gWorldLocation != 0xFFFFFFFF);

    CompileShadersProgram(ShaderCircle, "shaders/circle_fs.glsl", "shaders/circle_vs.glsl");
    gWorldLocation = glGetUniformLocation(ShaderCircle, "gWorld");
    assert(gWorldLocation != 0xFFFFFFFF);
}

static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    double speed_movement = 0.1 * region.z;
    double speed_rotation = 0.075;
    switch (key) {
        case GLFW_KEY_F:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            IsEnd = true;
            break;
        case GLFW_KEY_W:
            pipeline.camera.Params.WorldPos.z += speed_movement;
            break;
        case GLFW_KEY_S:
            pipeline.camera.Params.WorldPos.z -= speed_movement;
            break;
        case GLFW_KEY_D:
            pipeline.camera.Params.WorldPos.x += speed_movement;
            break;
        case GLFW_KEY_A:
            pipeline.camera.Params.WorldPos.x -= speed_movement;
            break;
        case GLFW_KEY_SPACE:
            pipeline.camera.Params.WorldPos.y += speed_movement;
            break;
        case GLFW_KEY_C:
            pipeline.camera.Params.WorldPos.y -= speed_movement;
            break;
        case GLFW_KEY_E:
            pipeline.camera.Params.Target.x += speed_rotation;
            break;
        case GLFW_KEY_Q:
            pipeline.camera.Params.Target.x -= speed_rotation;
            break;
    }
}

void InitializeGLFW(GLFWwindow* &window)
{
    #if DEBUG 
        std::cout << __LINE__ << std::endl;
    #endif

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    Vector3f CameraPos(0.0f, 0.1f, -region.z * 2 - 1);
    Vector3f CameraTarget(0.0f, 0.0f, 1.0f);
    Vector3f CameraUp(0.0f, 1.0f, 0.0f);
    pipeline.camera.SetCamera(CameraPos, CameraTarget, CameraUp);
    pipeline.camera.SetPerspectiveProj(60.0f, width, height, 0.5f, 1000.0f);
    pipeline.object.SetScale(0, 0, 0);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "NBody", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, KeyboardCB);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);


    CompileShaders();
    TextureFloor = LoadTexures("img/floor.jpg");
    region.VSet(1.0f, 1.0f, 1.0f);
}
